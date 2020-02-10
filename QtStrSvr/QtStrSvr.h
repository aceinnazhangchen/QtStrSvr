#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtStrSvr.h"
#include "ClientModuleUI.h"

class QtStrSvr : public QMainWindow
{
	Q_OBJECT

public:
	QtStrSvr(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtStrSvrClass ui;
	ClientModuleUI* w_ClientUi;
};
