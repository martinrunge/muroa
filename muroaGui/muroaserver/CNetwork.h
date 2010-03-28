#ifndef CNETWORK_H
#define CNETWORK_H

#include <QObject>
#include <QtNetwork>
#include <QTcpServer>

class CNetwork : public QObject
{
    Q_OBJECT;
public:
    CNetwork();
    ~CNetwork();

    int listen(int portNr);

signals:
    void newConnection(QTcpSocket* socket);
    // void connectionClosed(QIODevice* ioDev);

public slots:
    void newConnection();

    //void connected();
    //void disconnected();
    //void error(QAbstractSocket::SocketError socketError);

private:



    int m_port_nr;

    QTcpServer *m_server;
    // QTcpSocket *m_socket;
};

#endif // CNETWORK_H
