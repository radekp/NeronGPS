#include <QtGui/QApplication>
#include "desktop/desktop.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TDesktop desktop;
    return a.exec();
}

