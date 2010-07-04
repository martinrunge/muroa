#ifndef CSTATEMACHINE_H
#define CSTATEMACHINE_H

#include <QObject>
#include <QTcpSocket>
// #include <QXmlInputSource>

#include "CStateMachine.h"
#include "CCommandBase.h"

class QXmlStreamReader;
class QXmlStreamWriter;

enum connectionState {
	e_disconnected,
	e_connected
};

class CConnection : public QObject
{
    Q_OBJECT;
public:
    CConnection();
    ~CConnection();

    inline void setNextlistModelPtr(CPlaylistModel* const nextlistModelPtr) {m_sm.setNextlistModelPtr(nextlistModelPtr); };
    inline void setPlaylistModelPtr(CPlaylistModel* const playlistModelPtr) {m_sm.setPlaylistModelPtr(playlistModelPtr); };
    inline void setColletionModelPtr(CCollectionModel* const collectionModelPtr) {m_sm.setCollectionModelPtr(collectionModelPtr); };

signals:
    void connectionStatusChanged(enum connectionState state);
    void progressSig(int done, int total);

public slots:
    void close();
    void open(QString host, int port);

    void connected();
    void disconnected();
    void error();

    void play();
    void stop();

    void addSong(QString artist, QString album, QString title);
    void getCollection();
    void getCollection(int knownRevision);

    void getPlaylist();
    void getPlaylist(int knownRevision);

    void getNextlist();
    void getNextlist(int knownRevision);

    void sendCommand(CCommandBase* cmd);

    void readyRead();
    void test();

    void progress(int done, int total);


private:
    QTcpSocket m_socket;
    CStateMachine m_sm;
    QXmlStreamReader *m_xml_reader;
    QXmlStreamWriter *m_xml_writer;

    int m_state;
    int m_xml_depth;

};

#endif // CSTATEMACHINE_H
