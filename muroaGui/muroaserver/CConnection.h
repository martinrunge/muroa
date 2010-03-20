#ifndef CHWTEST_H
#define CHWTEST_H

#include <QObject>
#include <QtNetwork>

#include "CContentHandler.h"
#include "CStateMachine.h"
#include "CSession.h"

class QXmlInputSource;
class QXmlSimpleReader;
class QIODevice;
class QTcpSocket;

// class CSession;

class CConnection : public QObject
{
    Q_OBJECT;
public:
    CConnection(QTcpSocket* socket);
    ~CConnection();

    void sendCollection(int knownRevision = -1);
    void sendPlaylist(int knownRevision = -1);
    void sendNextlist(int knownRevision = -1);

signals:
    void connectionStatusChanged(QString message);
    void connectionClosed(CConnection* self);


public slots:
    // void newConnection(QIODevice* ioDev);
    // void connectionClosed(QIODevice* ioDev);

    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);


    void readyRead();

//    inline void setCollection(CCollection* collection) { m_session->setCollectionPtr(collection); };
    inline void setSessionPtr(CSession * session)
    {
    	m_session = session;
    	m_sm.setSessionPtr(m_session);
    };

private:
    QTcpSocket* m_socket;
    //CNetwork *m_net;
    // QIODevice* m_io_dev;
    QXmlStreamWriter* m_xml_writer;

    CStateMachine m_sm;

    CContentHandler m_contentHandler;
    CSession* m_session;
};

#endif // CHWTEST_H
