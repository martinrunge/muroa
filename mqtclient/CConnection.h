#ifndef CSTATEMACHINE_H
#define CSTATEMACHINE_H

#include <QObject>
#include <QTcpSocket>
#include <QSettings>

#include <muroaxml/CMuroaXml.h>
#include <cmds/CCmdBase.h>

enum connectionState {
	e_disconnected,
	e_connected
};

class CConnection : public QObject, CMuroaXml
{
    Q_OBJECT;
public:
    CConnection();
    ~CConnection();

    void onDataToSend(const char *data, int length);
<<<<<<< HEAD

	void onListSessions(std::vector<std::string> sessions);
	void onJoinSession(std::string sessionName);
=======
	void onJoinSession(uint32_t sessionID);
>>>>>>> 485bf2644495e28e4bf913a061d7967e02fa0fc0
	void onLeaveSession();

	void onPlay();
	void onPause();
	void onStop();
	void onNext();
	void onPrev();

	void onStateChanged(int newState);
	void onProgress(uint32_t jobID, int progress);
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

signals:
    void connectionStatusChanged(enum connectionState state);
    void progressSig(int done, int total);

public slots:
    void close();
    void open(QString host, int port);

    void connected();
    void disconnected();
    void error();

    void sendCommand(muroa::CCmdBase* cmd);

    void readyRead();


private:
    QTcpSocket m_socket;

    int m_state;
    int m_xml_depth;

    QSettings m_settings;


};

#endif // CSTATEMACHINE_H
