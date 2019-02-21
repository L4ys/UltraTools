#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_UltraTools.h"

class UltraTools : public QMainWindow
{
	Q_OBJECT

public:
	UltraTools(QWidget *parent = Q_NULLPTR);

private:
	Ui::UltraToolsClass ui;
};
