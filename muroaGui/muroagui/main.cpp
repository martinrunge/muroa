#include "CMuroaGui.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMuroaGui w;
    w.show();
    return a.exec();
}
