#ifndef CSTATEMACHINE_H
#define CSTATEMACHINE_H

#include <QObject>
#include <QTcpSocket>
#include <QSettings>

#include "CClientSM.h"

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

	void onPlay(uint32_t jobID = 0);
	void onPause(uint32_t jobID = 0);
	void onStop(uint32_t jobID = 0);
	void onNext(uint32_t jobID = 0);
	void onPrev(uint32_t jobID = 0);

	void onStateChanged(int newState);
	void onScanCollection(uint32_t jobID) {};
	void onProgress(uint32_t jobID, int progress, int total);
	void onFinished(uint32_t jobID);
	void onError(uint32_t jobID, int errorCode, std::string description);

	void onGetCollection(uint32_t jobID, unsigned knownRev );
	void onGetPlaylist(uint32_t jobID, unsigned knownRev );
	void onGetNextlist(uint32_t jobID, unsigned knownRev );
	void onGetSessionState( uint32_t jobID, unsigned knownRev );

	void onCollection(uint32_t jobID, unsigned  diffFromRev, std::string collection);
	void onPlaylist(uint32_t jobID, unsigned  diffFromRev, std::string playlist);
	void onNextlist(uint32_t jobID, unsigned  diffFromRev, std::string nextlist);

	void onEditCollection(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string collectionDiff );
	void onEditPlaylist(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string playlistDiff );
	void onEditNextlist(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string nextlistDiff );
	void onEditSessionState(uint32_t jobID, unsigned fromRev, unsigned toRev, std::string sessionStateDiff );

	void onGetSessionClients(uint32_t jobID);
	void onGetUnassignedClients(uint32_t jobID);
	void onAddClient(uint32_t jobID, std::string name);
	void onRmClient(uint32_t jobID, std::string name);
	void onEnableClient(uint32_t jobID, std::string name);
	void onDisableClient(uint32_t jobID, std::string name);

	void onStartSession(){ };
	void onEndSession(){ };
	void onXmlVersion(){ };

	inline CClientSM* getSessionSMPtr() { return &m_sm; };
	inline CSession* getSession() {return m_session; };

signals:
    void connectionStatusChanged(enum connectionState state);
    void progressSig(int done, int total);

public slots:
    void close();
    void open(QString host, int port);

    void connected();
    void disconnected();
    void error();

    void play(uint32_t jobID = 0);
    void stop(uint32_t jobID = 0);
    void next(uint32_t jobID = 0);
    void prev(uint32_t jobID = 0);

    void sendCommand(CmdBase* cmd);

    void readyRead();


private:

    void doJoinSession(std::string name);

    QTcpSocket m_socket;
    CClientSM m_sm;

    int m_state;
    int m_xml_depth;

    QSettings m_settings;

    CSession* const m_session;

};

#endif // CSTATEMACHINE_H
