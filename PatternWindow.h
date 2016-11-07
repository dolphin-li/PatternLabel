#ifndef MESHWINDOW_H
#define MESHWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_patternwindow.h"
class PatternLabelUI;
class PatternImageInfo;
class PatternWindow : public QMainWindow
{
	Q_OBJECT

public:
	PatternWindow(QWidget *parent = 0);
	~PatternWindow();

	void updateImages();
	void setMainUI(PatternLabelUI* ui) { m_mainUI = ui; }
	public slots:
	void listItemSelectionChanged();
	void listItemClicked(QListWidgetItem *);
protected:
	void resizeEvent(QResizeEvent* ev);
private:
	Ui_PatternWindow ui;
	int m_itemId_imgId;
//////////////////////////////////////////////////////////////////////////
protected:
	QVector<QSharedPointer<QListWidgetItem>> m_icons;
	PatternLabelUI* m_mainUI;
	PatternImageInfo* m_lastInfo;
};

#endif // SEWINGEDITOR_H
