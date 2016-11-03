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

private:
	Ui::PatternLabelUIClass ui;
};

#endif // PATTERNLABELUI_H
