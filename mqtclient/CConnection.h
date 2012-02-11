#ifndef CSTATEMACHINE_H
#define CSTATEMACHINE_H

#include <QObject>
#include <QTcpSocket>
#include <QSettings>

#include "CSessionSM.h"

#include <muroaxml/CMuroaXml.h>
#include <cmds/Cmd.h>

class CSession;

enum connectionState {
	e_disconnected,
	e_connected
};

class CConnection : public QObject, public CMuroaXml
{
    Q_OBJECT;
public:
    CConnection(CSession* const session);
    ~CConnection();

    void onDataToSend(const char *data, int length);

	void onListSessions(std::vector<std::string> sessions);
	void onJoinSession(std::string sessionName);
	void onLeaveSession();

	void onPlay();
	void onPause();
	void onStop();
	void onNext();
	void onPrev();

	void onStateChanged(int newState);
	void onScanCollection(uint32_t jobID) {};
	void onProgress(uint32_t jobID, int progress);
	void onFinished(uint32_t jobID);
	void onError(uint32_t jobID, int errorCode, std::string description);

	void onGetCollection( unsigned knownRev );
	void onGetPlaylist( unsigned knownRev );
	void onGetNextlist( unsigned knownRev );

	void onCollection(unsigned  diffFromRev, std::string collection);
	void onPlaylist(unsigned  diffFromRev, std::string playlist);
	void onNextlist(unsigned  diffFromRev, std::string nextlist);

	void onEditCollection( unsigned fromRev, std::string collectionDiff );
	void onEditPlaylist( unsigned fromRev, std::string playlistDiff );
	void onEditNextlist( unsigned fromRev, std::string nextlistDiff );

	void onStartSession(){ };
	void onEndSession(){ };
	void onXmlVersion(){ };

	inline CSessionSM* getSessionSMPtr() { return &m_sm; };

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
    void next();
    void prev();

    void sendCommand(CmdBase* cmd);

    void readyRead();


private:

    void doJoinSession(std::string name);

    QTcpSocket m_socket;
    CSessionSM m_sm;

    int m_state;
    int m_xml_depth;

    QSettings m_settings;

    CSession* const m_session;

};

#endif // CSTATEMACHINE_H
