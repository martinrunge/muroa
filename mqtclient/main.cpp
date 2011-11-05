#include "CQtClient.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{

    QCoreApplication::setOrganizationName("muroa");
    QCoreApplication::setOrganizationDomain("muroa.org");
    QCoreApplication::setApplicationName("muroagui");

    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    CQtClient w;
    w.show();
    return a.exec();
}
