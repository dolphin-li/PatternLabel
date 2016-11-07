#ifndef PATTERNLABELUI_H
#define PATTERNLABELUI_H

#include <QtWidgets/QMainWindow>
#include "ui_patternlabelui.h"
#include <QMutex>

class XmlSaveThread;
class PatternWindow;
class PatternLabelUI : public QMainWindow
{
	Q_OBJECT

public:
	PatternLabelUI(QWidget *parent = 0);
	~PatternLabelUI();
	void requireSaveXml();
	public slots:
	void on_actionLoad_image_list_triggered();
	void on_actionLoad_jd_image_list_triggered();
	void on_actionLoad_xml_triggered();
	void on_actionSave_xml_triggered();
	void on_actionCollect_pattern_xmls_triggered();
	void on_actionLoad_pattern_xml_triggered();
	void on_pbPrevIndex_clicked();
	void on_pbNextIndex_clicked();
	void on_pbLastImageThisIndex_clicked();
	void on_pbNextImageThisIndex_clicked();
	void pbGroupRbTypesClicked(int buttonID);
	void on_sbCurIndex_valueChanged(int v);
protected:
	void setupRadioButtons();
	void updateByIndex(int index, int imgId);
	void closeEvent(QCloseEvent* ev);
private:
	Ui::PatternLabelUIClass ui;
	QMap<QString, QSharedPointer<QButtonGroup>> m_rbTypes;
	bool m_updateSbIndex;
	XmlSaveThread* m_xmlSaveThread;
	QSharedPointer<PatternWindow> m_patternWindow;
};

#endif // PATTERNLABELUI_H
