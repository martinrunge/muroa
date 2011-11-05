#ifndef CSTATEMACHINE_H
#define CSTATEMACHINE_H

#include <QObject>
#include <QTcpSocket>

#include <cmds/CCmdBase.h>

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

};

#endif // CSTATEMACHINE_H
