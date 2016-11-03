#ifndef PATTERNLABELUI_H
#define PATTERNLABELUI_H

#include <QtWidgets/QMainWindow>
#include "ui_patternlabelui.h"

class PatternLabelUI : public QMainWindow
{
	Q_OBJECT

public:
	PatternLabelUI(QWidget *parent = 0);
	~PatternLabelUI();
	public slots:
	void on_actionLoad_image_list_triggered();
	void on_actionLoad_xml_triggered();
	void on_actionSave_xml_triggered();
	void on_pbPrevIndex_clicked();
	void on_pbNextIndex_clicked();
	void on_pbLastImageThisIndex_clicked();
	void on_pbNextImageThisIndex_clicked();
	void pbGroupRbTypesClicked(int buttonID);
protected:
	void setupRadioButtons();
	void updateByIndex(int index, int imgId);
private:
	Ui::PatternLabelUIClass ui;
	QMap<QString, QSharedPointer<QButtonGroup>> m_rbTypes;
};

#endif // PATTERNLABELUI_H
