#include <QtGui/QApplication>
#include "include/nerongps.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TNeronGPS neronGPS;
    return a.exec();
}
