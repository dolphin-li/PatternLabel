#include "patternlabelui.h"
#include "global_data_holder.h"
#include "PatternImageInfo.h"
#include <iostream>
#include <QFileDialog>
#include <QRadioButton>
#include <QGridLayout>

const static char* g_default_folder = "//dongping-pc1/d/dongping/BodyReconstruct/sewingPatterns/burdastyle_data";

PatternLabelUI::PatternLabelUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

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

void PatternLabelUI::on_actionLoad_image_list_triggered()
{
	try
	{
		QString name = QFileDialog::getOpenFileName(this, "load image list", g_default_folder, "*.txt");
		if (name.isEmpty())
			return;
		g_dataholder.loadImageList(name.toStdString());
		updateByIndex(0, 0);
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
		QString name = QFileDialog::getOpenFileName(this, "load xml", g_default_folder, "*.xml");
		if (name.isEmpty())
			return;
		g_dataholder.loadXml(name.toStdString());
		updateByIndex(0, 0);
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
		QString name = QFileDialog::getOpenFileName(this, "load xml", g_default_folder, "*.xml");
		if (name.isEmpty())
			return;
		if (!name.endsWith(".xml"))
			name.append(".xml");
		std::string tmp, path;
		ldp::fileparts(name.toStdString(), path, tmp, tmp);
		if (ldp::validWindowsPath(path) != ldp::validWindowsPath(g_dataholder.m_rootPath))
			printf("warning: you seemed to save xml to a wrong folder!\n");
		g_dataholder.saveXml(name.toStdString());
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
		g_dataholder.saveXml(ldp::fullfile(g_dataholder.m_rootPath, "patterns.xml"));
	g_dataholder.m_curIndex = (index + g_dataholder.m_imgInfos.size()) % g_dataholder.m_imgInfos.size();
	const auto& info = g_dataholder.m_imgInfos.at(g_dataholder.m_curIndex);
	g_dataholder.m_curIndex_imgIndex = (imgId + info.numImages()) % info.numImages();
	QImage img(info.getImageName(g_dataholder.m_curIndex_imgIndex).c_str());
	ui.widget->setColorImage(img);

	std::vector<std::string> typeNames = PatternImageInfo::attributeNames();
	for (auto name : typeNames)
	{
		auto type = info.getAttributeType(name);
		auto& btnGroup = m_rbTypes[name.c_str()];
		for (auto &btn : btnGroup->buttons())
		{
			if (btn->text() == info.getAttributeType(name).c_str())
			{
				btn->setChecked(true);
				break;
			}
		} // end for btn
	} // end for anme

	setWindowTitle(QString().sprintf("[%s]: %d/%d; %d/%d", g_dataholder.m_rootPath.c_str(),
		g_dataholder.m_curIndex, g_dataholder.m_imgInfos.size(),
		g_dataholder.m_curIndex_imgIndex, info.numImages()));
}

void PatternLabelUI::setupRadioButtons()
{
	m_rbTypes.clear();
	std::vector<std::string> typeNames = PatternImageInfo::attributeNames();
	for (auto name : typeNames)
	{
		m_rbTypes.insert(name.c_str(), QSharedPointer<QButtonGroup>(new QButtonGroup()));
		QGroupBox* box = new QGroupBox(ui.dockWidgetContentsRight);
		box->setTitle(name.c_str());
		ui.dockWidgetContentsRight->layout()->addWidget(box);
		QGridLayout* layout = new QGridLayout(box);
		box->setLayout(layout);
		const auto& types = PatternImageInfo::attributeTypes(name);
		for (size_t i = 0; i < types.size(); i++)
		{
			QRadioButton* btn = new QRadioButton(box);
			btn->setText(types[i].c_str());
			layout->addWidget(btn, i / 2, i % 2);
			m_rbTypes[name.c_str()]->addButton(btn);
		} // type
		connect(m_rbTypes[name.c_str()].data(), SIGNAL(buttonClicked(int)), this, SLOT(pbGroupRbTypesClicked(int)));
	} // name
}

void PatternLabelUI::pbGroupRbTypesClicked(int buttonID)
{
	try
	{
		auto& info = g_dataholder.m_imgInfos.at(g_dataholder.m_curIndex);
		std::vector<std::string> typeNames = PatternImageInfo::attributeNames();
		for (const auto& iter : m_rbTypes.toStdMap())
		{
			auto name = iter.first.toStdString();
			for (const auto& btn : iter.second->buttons())
			{
				if (btn->isChecked())
				{
					info.setAttributeType(name, btn->text().toStdString());
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
