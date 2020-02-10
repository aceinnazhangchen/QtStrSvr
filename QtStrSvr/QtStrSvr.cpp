#include "QtStrSvr.h"

QtStrSvr::QtStrSvr(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	w_ClientUi = new ClientModuleUI(this);
	ui.verticalLayout->addWidget(w_ClientUi);
}
