#ifndef CHWTEST_H
#define CHWTEST_H

#include <QObject>
#include "cnetwork.h"
#include "ccontenthandler.h"
#include "cstatemachine.h"


class QXmlInputSource;
class QXmlSimpleReader;
class QIODevice;

class CConnection : public QObject
{
    Q_OBJECT;
public:
    CConnection();
    ~CConnection();

signals:
    void connectionStatusChanged(QString message);

public slots:
    void newConnection(QIODevice* ioDev);
    void connectionClosed(QIODevice* ioDev);

    void readyRead();


private:
    CNetwork *m_net;
    QIODevice* m_io_dev;
    QXmlStreamReader* m_xml_reader;
    QXmlStreamWriter* m_xml_writer;

    CStateMachine m_sm;

    CContentHandler m_contentHandler;

};

#endif // CHWTEST_H
