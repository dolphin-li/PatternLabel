#include "patternlabelui.h"
#include "global_data_holder.h"
#include "PatternImageInfo.h"
#include <iostream>
#include <QFileDialog>
#include <QRadioButton>
#include <QGridLayout>
#include <QPixmapCache>
#include <QThread>
#include <QMutex>
#include <QShortcut>
#include <QMessageBox>
#include "PatternWindow.h"

class QDebugStream : public std::basic_streambuf<char>
{
public:
	QDebugStream(std::ostream &stream, QConsole* text_edit, QColor color) : m_stream(stream)
	{
		log_window = text_edit;
		m_old_buf = stream.rdbuf();
		stream.rdbuf(this);
		m_color = color;
	}
	~QDebugStream()
	{
		// output anything that is left
		if (!m_string.empty())
			log_window->append(m_string.c_str());

		m_stream.rdbuf(m_old_buf);
	}

protected:
	virtual int_type overflow(int_type v)
	{
		if (v == '\n')
		{
			log_window->append(m_string.c_str());
			m_string.erase(m_string.begin(), m_string.end());
		}
		else
			m_string += v;

		return v;
	}

	virtual std::streamsize xsputn(const char *p, std::streamsize n)
	{
		m_string.append(p, p + n);

		int pos = 0;
		while (pos != std::string::npos)
		{
			pos = m_string.find('\n');
			if (pos != std::string::npos)
			{
				std::string tmp(m_string.begin(), m_string.begin() + pos);
				log_window->setTextColor(m_color);
				log_window->append(tmp.c_str());
				m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
			}
		}
		return n;
	}
private:
	std::ostream &m_stream;
	std::streambuf *m_old_buf;
	std::string m_string;
	QConsole* log_window;
	QColor m_color;
};

class XmlSaveThread : public QThread
{
public:
	XmlSaveThread()
	{
		isSaving = false;
		needSave = false;
		needEnd = false;
	}
	void requireSave()
	{
		isSaving = false;
		needSave = true;
	}
	void requireEnd()
	{
		while (1) 
		{
			if (!isSaving)
			{
				std::cout  << "thread end" << std::endl;
				break;
			}
		}
		needEnd = true;
	}
private:
	bool isSaving;
	bool needSave;
	bool needEnd;
	QMutex mutex;
protected:
	virtual void run()
	{
		while (1)
		{
			if (!isSaving && needSave)
			{
				mutex.lock();
				isSaving = true;
				mutex.unlock();
				QFileInfo finfo;
				finfo.setFile(g_dataholder.m_rootPath, g_dataholder.m_xmlExportPureName);
#ifndef NDEBUG
				std::cout << "saving: " << finfo.absoluteFilePath().toStdString() << std::endl;
#endif
				g_dataholder.saveXml(finfo.absoluteFilePath());
#ifndef NDEBUG
				std::cout << "saved" << std::endl;
#endif
				mutex.lock();
				isSaving = false;
				needSave = false;
				mutex.unlock();
			} // end isSaving
			if (!isSaving && needEnd)
				break;
		} // end while 1
		std::cout << "SaveXmlThread ended" << std::endl;
	}
}; // XmlSaveThread

PatternLabelUI::PatternLabelUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_updateSbIndex = true;
	new QDebugStream(std::cout, ui.console, Qt::gray);
	new QDebugStream(std::cerr, ui.console, Qt::red);
	new QShortcut(QKeySequence(Qt::Key_F11), this, SLOT(showFullScreen()));
	new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(showNormal()));
	try
	{
		m_patternWindow.reset(new PatternWindow());
		m_patternWindow->setMainUI(this);
		m_xmlSaveThread = new XmlSaveThread();
		m_xmlSaveThread->start();
		g_dataholder.init();
		setupRadioButtons();
	} catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
	} catch (...)
	{
		std::cerr << "..." << std::endl;
	}
}

PatternLabelUI::~PatternLabelUI()
{
	
}

void PatternLabelUI::closeEvent(QCloseEvent* ev)
{
	try
	{
		m_patternWindow->close();
		//updateByIndex(g_dataholder.m_curIndex_imgIndex, g_dataholder.m_curIndex_imgIndex);
		m_xmlSaveThread->requireEnd();
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_actionLoad_image_list_triggered()
{
	try
	{
		QString name = QFileDialog::getOpenFileName(this, "load image list", 
			g_dataholder.m_lastRun_RootDir, "*.txt");
		if (name.isEmpty())
			return;
		g_dataholder.loadImageList(name);
		ui.sbCurIndex->setMaximum(g_dataholder.m_imgInfos.size());
		updateByIndex(g_dataholder.m_lastRun_imgId, 0);
		m_updateSbIndex = false;
		ui.sbCurIndex->setValue(g_dataholder.m_curIndex);
		m_updateSbIndex = true;
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_actionLoad_jd_image_list_triggered()
{
	try
	{
		QString name = QFileDialog::getOpenFileName(this, "load jd image list",
			g_dataholder.m_lastRun_RootDir, "*_imgId.xlsx");
		if (name.isEmpty())
			return;
		g_dataholder.loadJdImageList(name);
		ui.sbCurIndex->setMaximum(g_dataholder.m_imgInfos.size());
		updateByIndex(g_dataholder.m_lastRun_imgId, 0);
		m_updateSbIndex = false;
		ui.sbCurIndex->setValue(g_dataholder.m_curIndex);
		m_updateSbIndex = true;
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::requireSaveXml()
{
	if (g_dataholder.m_addPatternMode)
		return;
	if (g_dataholder.m_curIndex >= 0 && g_dataholder.m_curIndex_imgIndex >= 0)
		m_xmlSaveThread->requireSave();
}

void PatternLabelUI::on_actionLoad_xml_triggered()
{
	try
	{
		QString name = QFileDialog::getOpenFileName(this, "load xml",
			g_dataholder.m_lastRun_RootDir, "*.xml");
		if (name.isEmpty())
			return;
		g_dataholder.loadXml(name);
		g_dataholder.saveXml(name+"_backup_loaded");
		ui.sbCurIndex->setMaximum(g_dataholder.m_imgInfos.size());
		updateByIndex(g_dataholder.m_lastRun_imgId, 0);
		m_updateSbIndex = false;
		ui.sbCurIndex->setValue(g_dataholder.m_curIndex);
		m_updateSbIndex = true;

		if (g_dataholder.m_patternInfos.size())
		{
			m_patternWindow->show();
			m_patternWindow->updateImages();
		}
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_sbCurIndex_valueChanged(int v)
{
	if (!m_updateSbIndex)
		return;
	try
	{
		updateByIndex(v, 0);
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_actionSave_xml_triggered()
{
	try
	{
		QString name = QFileDialog::getOpenFileName(this, "load xml",
			g_dataholder.m_lastRun_RootDir, "*.xml");
		if (name.isEmpty())
			return;
		if (!name.endsWith(".xml"))
			name.append(".xml");
		QFileInfo rinfo(name), linfo(g_dataholder.m_rootPath);
		if (rinfo.absoluteDir() != linfo.absoluteDir())
			std::cout << "warning: you seemed to save xml to a wrong folder!\n";
		g_dataholder.saveXml(name);
		std::cout << "saved: " << name.toStdString() << std::endl;
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_actionCollect_pattern_xmls_triggered()
{
	try
	{
		QString name = QFileDialog::getExistingDirectory(this, "collect pattern xml",
			g_dataholder.m_lastRun_PatternDir);
		if (name.isEmpty())
			return;
		g_dataholder.collect_labelded_patterns(name);
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_actionLoad_pattern_xml_triggered()
{
	try
	{
		QString name = QFileDialog::getOpenFileName(this, "load pattern xml",
			g_dataholder.m_lastRun_PatternDir, "*.xml");
		if (name.isEmpty())
			return;
		g_dataholder.loadPatternXml(name);
		m_patternWindow->show();
		m_patternWindow->updateImages();
		
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_actionUnique_patterns_triggered()
{
	try
	{
		g_dataholder.savePatternXml(g_dataholder.m_inputPatternXmlName+"_before_unique");
		std::cout << "patterns backup done" << std::endl;
		g_dataholder.uniquePatterns();
		g_dataholder.savePatternXml(g_dataholder.m_inputPatternXmlName);
		std::cout << "patterns saved: " << g_dataholder.m_inputPatternXmlName.toStdString() << std::endl;

	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_actionSave_pattern_xml_triggered()
{
	try
	{
		QString savename = QFileDialog::getSaveFileName(this, "save pattern xml",
			g_dataholder.m_lastRun_PatternDir, "*.xml");
		if (savename.isEmpty())
			return;
		if (!savename.endsWith(".xml"))
			savename.append(".xml");
		QFileInfo saveinfo(savename), linfo(g_dataholder.m_inputPatternXmlName);
		auto savepath = saveinfo.absolutePath();
		if (saveinfo.absoluteDir() != linfo.absoluteDir())
		{
			std::cout << "warning: you seemed to save xml to a wrong folder!\n";
			// try to fix the path issue
			for (auto& info : g_dataholder.m_patternInfos)
			{
				if (info.numImages() == 0)
					throw std::exception("error: no image in the pattern to save!");
				auto imgname = info.getImageName(0);
				QFileInfo imginfo(imgname);
				auto imgpath = imginfo.absolutePath();
				if (imgpath.left(savepath.size()) != savepath)
					throw std::exception("error: we cannot save xml in such a path!");
				auto base = QDir::cleanPath(imgpath.right(imgpath.size() - savepath.size() - 1));
				info.setBaseName(base);
			} // end for info
		} // end if pattern dir not matched
		g_dataholder.savePatternXml(savename);
		std::cout << "patterns saved: " << savename.toStdString() << std::endl;

	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_pbPrevIndex_clicked()
{
	try
	{
		updateByIndex(g_dataholder.m_curIndex - 1, 0);
		m_updateSbIndex = false;
		ui.sbCurIndex->setValue(g_dataholder.m_curIndex);
		m_updateSbIndex = true;
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_pbNextIndex_clicked()
{
	try
	{
		updateByIndex(g_dataholder.m_curIndex + 1, 0);
		m_updateSbIndex = false;
		ui.sbCurIndex->setValue(g_dataholder.m_curIndex);
		m_updateSbIndex = true;
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_pbLastImageThisIndex_clicked()
{
	try
	{
		updateByIndex(g_dataholder.m_curIndex, g_dataholder.m_curIndex_imgIndex - 1);
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_pbNextImageThisIndex_clicked()
{
	try
	{
		updateByIndex(g_dataholder.m_curIndex, g_dataholder.m_curIndex_imgIndex + 1);
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_actionExport_training_data_triggered()
{
	try
	{
		QStringList names = QFileDialog::getOpenFileNames(this, "load pattern xml",
			g_dataholder.m_lastRun_RootDir, "*.xml");
		if (names.isEmpty())
			return;
		g_dataholder.exportPatternTrainingData(names);
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_actionRemove_other_triggered()
{
	try
	{
		int ret = QMessageBox::warning(this, "Remove", "Are you sure to remove all \"other\" typed images?",
			QMessageBox::Button::Cancel, QMessageBox::Button::Ok);
		if (ret != QMessageBox::Ok)
			return;
		auto tmps = g_dataholder.m_imgInfos;
		g_dataholder.m_imgInfos.clear();
		for (const auto& info : tmps)
		{
			if (info.getAttributeType("cloth-types") != "other")
				g_dataholder.m_imgInfos.push_back(info);
		}
		g_dataholder.m_curIndex = 0;
		g_dataholder.m_curIndex_imgIndex = 0;
		ui.sbCurIndex->setValue(g_dataholder.m_curIndex);
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::updateByIndex(int index, int imgId)
{
	if (g_dataholder.m_imgInfos.size() == 0)
		return;
	requireSaveXml();
	g_dataholder.m_curIndex = (index + g_dataholder.m_imgInfos.size()) % g_dataholder.m_imgInfos.size();
	g_dataholder.m_lastRun_imgId = g_dataholder.m_curIndex;
	const auto& info = g_dataholder.m_imgInfos.at(g_dataholder.m_curIndex);
	g_dataholder.m_curIndex_imgIndex = (imgId + info.numImages()) % info.numImages();
	ui.widget->setColorImage(info.getImage(g_dataholder.m_curIndex_imgIndex)->toImage());

	QVector<QString> typeNames = PatternImageInfo::attributeNames();
	for (auto name : typeNames)
	{
		auto type = info.getAttributeType(name);
		auto& btnGroup = m_rbTypes[name];
		for (auto &btn : btnGroup->buttons())
		{
			if (btn->text() == info.getAttributeType(name))
			{
				btn->setChecked(true);
				break;
			}
		} // end for btn
	} // end for anme

	setWindowTitle(g_dataholder.m_rootPath + QString().sprintf("]: %d/%d; %d/%d",
		g_dataholder.m_curIndex, g_dataholder.m_imgInfos.size(),
		g_dataholder.m_curIndex_imgIndex, info.numImages()));

	if (!m_patternWindow->isHidden())
		m_patternWindow->updateImages();
}

void PatternLabelUI::setupRadioButtons()
{
	m_rbTypes.clear();
	QVector<QString> typeNames = PatternImageInfo::attributeNames();
	for (auto name : typeNames)
	{
		m_rbTypes.insert(name, QSharedPointer<QButtonGroup>(new QButtonGroup()));
		QGroupBox* box = new QGroupBox(ui.dockWidgetContentsRight);
		box->setTitle(name);
		ui.dockWidgetContentsRight->layout()->addWidget(box);
		QGridLayout* layout = new QGridLayout(box);
		box->setLayout(layout);
		const auto& types = PatternImageInfo::attributeTypes(name);
		for (size_t i = 0; i < types.size(); i++)
		{
			QRadioButton* btn = new QRadioButton(box);
			btn->setText(types[i]);
			layout->addWidget(btn, i / 3, i % 3);
			m_rbTypes[name]->addButton(btn);
		} // type
		connect(m_rbTypes[name].data(), SIGNAL(buttonClicked(int)), 
			this, SLOT(pbGroupRbTypesClicked(int)));
	} // name
}

void PatternLabelUI::pbGroupRbTypesClicked(int buttonID)
{
	try
	{
		auto& info = g_dataholder.m_imgInfos.at(g_dataholder.m_curIndex);
		QVector<QString> typeNames = PatternImageInfo::attributeNames();
		for (const auto& iter : m_rbTypes.toStdMap())
		{
			auto name = iter.first;
			for (const auto& btn : iter.second->buttons())
			{
				if (btn->isChecked())
				{
					info.setAttributeType(name, btn->text());
					break;
				}
			}
		} // end for anme
		if (!m_patternWindow->isHidden())
			m_patternWindow->updateImages();
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}

void PatternLabelUI::on_cbAddPatternMode_clicked()
{
	g_dataholder.m_addPatternMode = ui.cbAddPatternMode->isChecked();
	if (g_dataholder.m_addPatternMode)
		m_patternWindow->show();
}

void PatternLabelUI::on_pbAddPattern_clicked()
{
	try
	{
		if (!g_dataholder.m_addPatternMode)
			return;
		if (g_dataholder.m_curIndex < 0 || g_dataholder.m_curIndex >= g_dataholder.m_imgInfos.size())
			return;
		const auto& info = g_dataholder.m_imgInfos[g_dataholder.m_curIndex];
		g_dataholder.m_patternInfos.push_back(info);
		g_dataholder.m_namePatternMap.clear();
		for (auto& pattern : g_dataholder.m_patternInfos)
			g_dataholder.m_namePatternMap.insert(pattern.getBaseName(), qMakePair(&pattern, 0));
		if (!m_patternWindow->isHidden())
			m_patternWindow->updateImages();
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}
