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
			icon->setIcon(QIcon(*iter.value().first->getImage(m_itemId_imgId)));
			icon->setText(iter.value().first->getBaseName());
			icon->setToolTip(QString().sprintf("[%d] ", iter.value().second)+ 
				iter.value().first->getImageName(m_itemId_imgId));
			ui.listWidget->addItem(icon.data());
			m_icons.push_back(icon);
			selId = 0;
			m_lastInfo = iter.value().first;
		} // end if iter
	} // end if mapped
	// insert matched items, sorted by their frequency
	QVector<QPair<int, PatternImageInfo*>> matched;
	for (const auto& info : g_dataholder.m_patternInfos)
	{
		if (info != query_info)
			continue;
		const auto& iter = g_dataholder.m_namePatternMap.find(info.getBaseName());
		if (iter != g_dataholder.m_namePatternMap.end())
			matched.push_back(qMakePair(iter.value().second, iter.value().first));
	} // end for info
	qSort(matched.begin(), matched.end(), qGreater<QPair<int, PatternImageInfo*>>());
	for (const auto& match : matched)
	{
		const auto& info = *match.second;
		QSharedPointer<QListWidgetItem> icon(new QListWidgetItem());
		icon->setIcon(QIcon(*info.getImage(m_itemId_imgId)));
		icon->setText(info.getBaseName());
		icon->setToolTip(QString().sprintf("[%d] ", match.first) + info.getImageName(m_itemId_imgId));
		ui.listWidget->addItem(icon.data());
		m_icons.push_back(icon);
	} // end for info
	if (selId >= 0)
		ui.listWidget->setItemSelected(ui.listWidget->item(selId), true);
	setWindowTitle(g_dataholder.m_inputPatternXmlName + "]: " + QString().sprintf("%d", m_icons.size()));
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
	m_itemId_imgId = (m_itemId_imgId + 1) % iter.value().first->numImages();

	if (g_dataholder.m_curIndex >= g_dataholder.m_imgInfos.size()
		|| g_dataholder.m_curIndex < 0)
		return;
	if (m_itemId_imgId == 0)
	{
		auto& query_info = g_dataholder.m_imgInfos[g_dataholder.m_curIndex];
		query_info.setJdMappedPattern(iter.value().first->getBaseName());
		if (m_mainUI && m_lastInfo != iter.value().first)
		{
			m_mainUI->requireSaveXml();
			iter.value().second++;
			if (m_lastInfo)
				g_dataholder.m_namePatternMap.find(m_lastInfo->getBaseName()).value().second--;
			m_lastInfo = iter.value().first;
		}
	}

	while (iter.value().first->getImage(m_itemId_imgId)->width() == 0)
		m_itemId_imgId = (m_itemId_imgId + 1) % iter.value().first->numImages();
	item->setIcon(QIcon(*iter.value().first->getImage(m_itemId_imgId)));
	ui.listWidget->update();
}

void PatternWindow::resizeEvent(QResizeEvent* ev)
{
	ui.listWidget->update();
}
