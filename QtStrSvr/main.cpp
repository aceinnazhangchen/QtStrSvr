#include "QtStrSvr.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtStrSvr w;
	w.show();
	return a.exec();
}
