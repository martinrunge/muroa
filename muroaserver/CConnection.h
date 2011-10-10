#ifndef CHWTEST_H
#define CHWTEST_H

#include <QObject>
#include <QtNetwork>

#include "CContentHandler.h"
#include "CStateMachine.h"
#include "CSession.h"
#include "CCollection.h"
#include "Exceptions.h"

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

    void next();
    void prev();
    void play();
    void stop();

    void sendCollection();
    void sendCollection(CCollection<CCollectionItem>* collection);
    void sendCollectionDiff(int diffFromRev);
    void sendCollectionDiff(int revision, int diffFromRev, QString diff);

    void sendPlaylist();
    void sendPlaylist(CCollection<CPlaylistItem>* playlist);
    void sendPlaylistDiff(int diffFromRev);
    void sendPlaylistDiff(int revision, int diffFromRev, QString diff);

    void sendNextlist();
    void sendNextlist(CCollection<CPlaylistItem>* nextlist);
    void sendNextlistDiff(int diffFromRev);
    void sendNextlistDiff(int revision, int diffFromRev, QString diff);

	int addCollectionRevFromDiff(QString* collectionDiff, int diffFromRev);
	int addPlaylistRevFromDiff(QString* playlistDiff, int diffFromRev);
	int addNextlistRevFromDiff(QString* nextlistDiff, int diffFromRev);

    void sendProgress(int done, int total);

    void reportError(QString error);

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
    	// m_sm.setSessionPtr(m_session);
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
