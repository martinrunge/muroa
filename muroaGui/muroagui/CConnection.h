#ifndef CSTATEMACHINE_H
#define CSTATEMACHINE_H

#include <QObject>
#include <QTcpSocket>
// #include <QXmlInputSource>

#include "CStateMachine.h"
#include "CCommandBase.h"

class QXmlStreamReader;
class QXmlStreamWriter;

class CConnection : public QObject
{
    Q_OBJECT;
public:
    CConnection();
    ~CConnection();

    inline void setPlaylistModelPtr(CPlaylistModel* const playlistModelPtr) {m_sm.setPlaylistModelPtr(playlistModelPtr); };
    inline void setColletionModelPtr(CCollectionModel* const collectionModelPtr) {m_sm.setCollectionModelPtr(collectionModelPtr); };

signals:
    void connectionStatusChanged(QString message);


public slots:
    void close();
    void open(QString host, int port);

    void connected();
    void disconnected();
    void error();

    void readyRead();

    void addSong(QString artist, QString album, QString title);
    void getCollection();
    void getCollection(int knownRevision);

    void getPlaylist();
    void getPlaylist(int knownRevision);

    void getNextlist();
    void getNextlist(int knownRevision);

    void sendCommand(const CCommandBase& cmd);

    void test();


private:
    QTcpSocket m_socket;
    CStateMachine m_sm;
    QXmlStreamReader *m_xml_reader;
    QXmlStreamWriter *m_xml_writer;

    int m_state;
    int m_xml_depth;

};

#endif // CSTATEMACHINE_H
