#include <QtGui/QApplication>
#include "maemo5/maemo5.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TMaemo5 maemo5;
    return a.exec();
}

