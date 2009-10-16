#ifndef CNETWORK_H
#define CNETWORK_H

#include <QObject>
#include <QtNetwork>
#include <QTcpServer>

class CNetwork : public QObject
{
    Q_OBJECT;
public:
    CNetwork(int portNr);
    ~CNetwork();

signals:
    void newConnection(QIODevice* ioDev);
    void connectionClosed(QIODevice* ioDev);

public slots:
    void newConnection();

    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);

private:

    void listen();

    int m_port_nr;

    QTcpServer *m_server;
    QTcpSocket *m_socket;
};

#endif // CNETWORK_H
