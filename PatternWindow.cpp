#include "PatternWindow.h"
#include "global_data_holder.h"
#include <QShortcut>
#include <qevent.h>
#include "patternlabelui.h"
PatternWindow::PatternWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_mainUI = nullptr;
	new QShortcut(QKeySequence(Qt::Key_F11), this, SLOT(showFullScreen()));
	new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(showNormal()));
	connect(ui.listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(listItemSelectionChanged()));
	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(listItemClicked(QListWidgetItem *)));
	m_lastInfo = nullptr;
}

PatternWindow::~PatternWindow()
{

}

void PatternWindow::updateImages()
{
	if (isHidden())
		return;
	if (g_dataholder.m_curIndex >= g_dataholder.m_imgInfos.size()
		|| g_dataholder.m_curIndex < 0)
		return;
	const auto& query_info = g_dataholder.m_imgInfos[g_dataholder.m_curIndex];
	m_icons.clear();
	ui.listWidget->clear();
	ui.listWidget->setIconSize(QSize(200, 300));
	m_itemId_imgId = 0;
	m_lastInfo = nullptr;

	// if mapped before, insert it firstly
	int selId = -1;
	if (!query_info.getJdMappedPattern().isEmpty())
	{
		const auto& iter = g_dataholder.m_namePatternMap.find(query_info.getJdMappedPattern());
		if (iter != g_dataholder.m_namePatternMap.end())
		{
			QSharedPointer<QListWidgetItem> icon(new QListWidgetItem());
			icon->setIcon(QIcon(*iter.value()->getImage(m_itemId_imgId)));
			icon->setText(iter.value()->getBaseName());
			icon->setToolTip(iter.value()->getImageName(m_itemId_imgId));
			ui.listWidget->addItem(icon.data());
			m_icons.push_back(icon);
			selId = 0;
			m_lastInfo = iter.value();
		} // end if iter
	} // end if mapped
	for (const auto& info : g_dataholder.m_patternInfos)
	{
		if (info != query_info)
			continue;
		QSharedPointer<QListWidgetItem> icon(new QListWidgetItem());
		icon->setIcon(QIcon(*info.getImage(m_itemId_imgId)));
		icon->setText(info.getBaseName());
		icon->setToolTip(info.getImageName(m_itemId_imgId));
		ui.listWidget->addItem(icon.data());
		m_icons.push_back(icon);
	} // end for info
	if (selId >= 0)
		ui.listWidget->setItemSelected(ui.listWidget->item(selId), true);
	std::cout << m_icons.size() << " images founded" << std::endl;
}

void PatternWindow::listItemSelectionChanged()
{
	m_itemId_imgId = -1;
}

void PatternWindow::listItemClicked(QListWidgetItem *item)
{
	const auto& iter = g_dataholder.m_namePatternMap.find(item->text());
	if (iter == g_dataholder.m_namePatternMap.end())
		return;
	m_itemId_imgId = (m_itemId_imgId + 1) % iter.value()->numImages();

	if (g_dataholder.m_curIndex >= g_dataholder.m_imgInfos.size()
		|| g_dataholder.m_curIndex < 0)
		return;
	if (m_itemId_imgId == 0)
	{
		auto& query_info = g_dataholder.m_imgInfos[g_dataholder.m_curIndex];
		query_info.setJdMappedPattern(iter.value()->getBaseName());
		if (m_mainUI && m_lastInfo != iter.value())
		{
			m_mainUI->requireSaveXml();
			m_lastInfo = iter.value();
		}
	}

	while (iter.value()->getImage(m_itemId_imgId)->width() == 0)
		m_itemId_imgId = (m_itemId_imgId + 1) % iter.value()->numImages();
	item->setIcon(QIcon(*iter.value()->getImage(m_itemId_imgId)));
	ui.listWidget->update();
}

void PatternWindow::resizeEvent(QResizeEvent* ev)
{
	ui.listWidget->update();
}