#include "muroagui.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    muroagui w;
    w.show();
    return a.exec();
}
