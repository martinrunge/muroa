#include "CMuroaServer.h"

#include "CSession.h"
#include "CNetwork.h"

#include <QMessageBox>
#include <QFile>


CMuroaServer::CMuroaServer(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(ui.action_next_Revision, SIGNAL(triggered()), this, SLOT(nextRevision()));

	m_net = new CNetwork(2678);
	connect(m_net, SIGNAL(newConnection(QTcpSocket*)), this, SLOT(newConnection(QTcpSocket*)));

	m_session = new CSession();

	statusBar()->addWidget(&m_connection_status_label);

	// connect(&m_connection, SIGNAL(connectionStatusChanged(QString)), this, SLOT(connectionStatusChanged(QString)));

	m_testfiles << "test/collection1.txt"
			    << "test/collection2.txt"
				<< "test/collection3.txt"
				<< "test/collection4.txt"
				<< "test/collection5.txt"
				<< "test/collection6.txt"
				<< "test/collection7.txt";

	readCollectionFile(m_testfiles[0]);
	// m_connection.setSessionPtr(m_session);
	//m_connection.setCollection(&m_collection);
}

CMuroaServer::~CMuroaServer()
{
	delete m_session;
	delete m_net;
}

void CMuroaServer::newConnection(QTcpSocket* socket)
{
	CConnection* conn = new CConnection(socket);
	m_session->addConnection(conn);
	connect(conn, SIGNAL(connectionStatusChanged(QString)), this, SLOT(connectionStatusChanged(QString)));
	connect(conn, SIGNAL(connectionClosed(CConnection*)), m_session, SLOT(connectionClosed(CConnection*)));

}

void CMuroaServer::connectionStatusChanged(QString status)
{
	m_connection_status_label.setText(status);
}

void CMuroaServer::readCollectionFile(QString filename)
{
	QFile collectionFile(filename);
	QString line;

	if (!collectionFile.open(QIODevice::ReadOnly)) // Open the file
	{
        QMessageBox::warning(this, "Load Collection", QString("Can not load collection from file %1").arg(filename));
	}

	QTextStream stream( &collectionFile ); // Set the stream to read from myFile
	QString text = stream.readAll();
	m_session->addCollectionRev( text );

	if(m_session->getPlaylistRevision() == 0)
	{
		// construct initial playlist
		CCollection<CCollectionItem>* col1 = m_session->getCollection(1);

		QString playlist;

		for(int i = 0; i < col1->size(); i++)
		{
			CCollectionItem item = col1->getItem(i);
			unsigned long hash = item.getHash();
			playlist.append(QString("%1\n").arg(hash));
		}
		m_session->addPlaylistRev( playlist );
	}

}


void CMuroaServer::nextRevision()
{
	readCollectionFile(m_testfiles[m_session->getCollectionRevision()]);
}
