#include "patternlabelui.h"
#include "global_data_holder.h"
#include "PatternImageInfo.h"
#include <iostream>
#include <QFileDialog>
#include <QRadioButton>
#include <QGridLayout>

PatternLabelUI::PatternLabelUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_updateSbIndex = true;
	try
	{
		g_dataholder.init();
		setupRadioButtons();
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "..." << std::endl;
	}
}

PatternLabelUI::~PatternLabelUI()
{

}

void PatternLabelUI::closeEvent(QCloseEvent* ev)
{
	try
	{
		updateByIndex(g_dataholder.m_curIndex_imgIndex, g_dataholder.m_curIndex_imgIndex);
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

void PatternLabelUI::on_actionLoad_xml_triggered()
{
	try
	{
		QString name = QFileDialog::getOpenFileName(this, "load xml",
			g_dataholder.m_lastRun_RootDir, "*.xml");
		if (name.isEmpty())
			return;
		g_dataholder.loadXml(name);
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
			printf("warning: you seemed to save xml to a wrong folder!\n");
		g_dataholder.saveXml(name);
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

void PatternLabelUI::updateByIndex(int index, int imgId)
{
	if (g_dataholder.m_imgInfos.size() == 0)
		return;
	if (g_dataholder.m_curIndex >= 0 && g_dataholder.m_curIndex_imgIndex >= 0)
	{
		QFileInfo finfo;
		finfo.setFile(g_dataholder.m_rootPath, g_dataholder.m_xmlExportPureName);
		g_dataholder.saveXml(finfo.absoluteFilePath());
	}
	g_dataholder.m_curIndex = (index + g_dataholder.m_imgInfos.size()) % g_dataholder.m_imgInfos.size();
	g_dataholder.m_lastRun_imgId = g_dataholder.m_curIndex;
	const auto& info = g_dataholder.m_imgInfos.at(g_dataholder.m_curIndex);
	g_dataholder.m_curIndex_imgIndex = (imgId + info.numImages()) % info.numImages();
	QImage img(info.getImageName(g_dataholder.m_curIndex_imgIndex));
	ui.widget->setColorImage(img);

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
			layout->addWidget(btn, i / 2, i % 2);
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
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "unknown error" << std::endl;
	}
}
