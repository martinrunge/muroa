#include "CMuroaServer.h"
#include "CConnection.h"

#include <QtGui>
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    int retval;
    QApplication a(argc, argv);
    CMuroaServer w;

    try
    {
        w.show();
        retval = a.exec();
    }
    catch(...)
    {
        qDebug() << QString("got exception");
        retval = 32;
    }

    return retval;
}
