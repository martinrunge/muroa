#include "CConnection.h"

#include <QDebug>

CConnection::CConnection()
{
	connect(&m_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(&m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

CConnection::~CConnection()
{
    disconnect(&m_socket, SIGNAL(connected()), this, SLOT(connected()));
    disconnect(&m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    disconnect(&m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void CConnection::open(QString host, int port)
{
    m_socket.connectToHost(host, port);
    emit connectionStatusChanged( e_connected );
}

void CConnection::close()
{
    m_socket.disconnectFromHost();
    emit connectionStatusChanged( e_disconnected );
}


void CConnection::connected()
{
    qDebug() << QString("CStateMachine::connected");


    emit connectionStatusChanged( e_connected );
}

void CConnection::disconnected()
{
    qDebug() << QString("CStateMachine::disconnected");
    emit connectionStatusChanged( e_disconnected );
}

void CConnection::error()
{

}

void CConnection::readyRead()
{
    int avail = m_socket.bytesAvailable();
    // qDebug() << QString("readyRead(): %1 bytes available").arg(avail);
    QByteArray data = m_socket.readAll();
    // qDebug() << QString("readyRead(): %1 bytes read").arg(data.size());
}
//  m_xml_reader->parse(&m_xml_src, true);
//}

void CConnection::sendCommand(muroa::CCmdBase* cmd)
{
    qDebug() << QString("sendCommand %1").arg(QString::fromUtf8(cmd->name().c_str()));
    delete cmd;
}

