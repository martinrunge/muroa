#include "CMuroaServer.h"

#include "CSession.h"
#include "CNetwork.h"

#include "mediascanner/CMsgOpenDb.h"
#include "mediascanner/CMsgScanDir.h"

#include <QMessageBox>
#include <QFile>


CMuroaServer::CMuroaServer(QWidget *parent)
    : QMainWindow(parent), m_mediaScannerCtrl(this), m_stateDB(0)
{
	ui.setupUi(this);
	connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(ui.action_next_Revision, SIGNAL(triggered()), this, SLOT(nextRevision()));
	connect(ui.actionScan_collection, SIGNAL(triggered()), this, SLOT(scanCollection()));

	int portnr = 16320 - 1;
	int retval;

	m_net = new CNetwork();

	do {
		retval = m_net->listen(++portnr);
	} while(retval == -1 );

	m_portNr = portnr;

	connect(m_net, SIGNAL(newConnection(QTcpSocket*)), this, SLOT(newConnection(QTcpSocket*)));

	m_session = new CSession();

	statusBar()->addWidget(&m_connection_status_label);

	readSettings();
	m_stateDB = new CStateDB(m_db_filename.toUtf8().constData());
	m_stateDB->open();

	m_stateDB->restoreCollection(m_session);

	// CCollection<CCollectionItem>* collection = m_collectionUpdater.walkTree( m_mediadir.toStdString() );
	// addCollectionRev(collection);

	connect(this, SIGNAL(progressSig(int)), ui.progressBar, SLOT(setValue(int)));
	connect(this, SIGNAL(responseSig(QString)), ui.response_label, SLOT(setText(QString)));

	m_dnssd.registerService("muroa", m_portNr);
}

CMuroaServer::~CMuroaServer()
{
	m_stateDB->saveSession(m_session);
	delete m_stateDB;
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


void CMuroaServer::scanProgress(int progress) {
	emit progressSig(progress);
}

void CMuroaServer::response(int id, int code, string message) {
	QString msg = QString("%1 %2 %3").arg(id).arg(code).arg(QString::fromUtf8(message.c_str()));
	emit responseSig(msg);
}


void CMuroaServer::addCollectionRev(CCollection<CCollectionItem>* collection)
{
	m_session->addCollectionRev( collection );

	if(m_session->getPlaylistRevision() == 0)
	{
		// construct initial playlist
		CCollection<CCollectionItem>* col1 = m_session->getCollection();

		QString playlist;

		for(int i = 0; i < col1->size(); i++)
		{
			CCollectionItem* item = col1->getItem(i);
			unsigned long hash = item->getHash();
			playlist.append(QString("%1\n").arg(hash));
		}
		m_session->addPlaylistRev( playlist );
		m_session->addNextlistRev( QString() );
	}
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
			CCollectionItem* item = col1->getItem(i);
			unsigned long hash = item->getHash();
			playlist.append(QString("%1\n").arg(hash));
		}
		m_session->addPlaylistRev( playlist );
		m_session->addNextlistRev( QString() );
	}

}


void CMuroaServer::nextRevision()
{
	// readCollectionFile(m_testfiles[m_session->getCollectionRevision()]);
}

void CMuroaServer::scanCollection() {
	CMsgOpenDb openDbMsg( m_db_filename.toUtf8().constData() );
	CMsgScanDir scanDirMsg( m_mediadir.toStdString() );
	m_mediaScannerCtrl.start();
	m_mediaScannerCtrl.sendEvent(&openDbMsg);
	// sleep(1);
	m_mediaScannerCtrl.sendEvent(&scanDirMsg);

}


void CMuroaServer::readSettings() {
	QSettings settings;

	if(settings.contains("mediadir") ) {
		m_mediadir = settings.value("mediadir").toString();
	}
	else {
		// mediadir was not mentioned in config file, make a dummy entry for it so the user can change that.
		settings.setValue("mediadir", "/path/to/media/collection");
	}

	QStringList mtypes;
	if(settings.contains("mediatypes") ) {
		mtypes = settings.value("mediatypes").toStringList();
	}
	else {
		// mediadir was not mentioned in config file, make a dummy entry for it so the user can change that.
		mtypes << ".mp3" << ".ogg";
		settings.setValue("mediatypes", QVariant(mtypes));
	}
	std::vector<string> types;
	types.reserve( mtypes.size());

	for(int i = 0; i < mtypes.size(); i++) {
		types.push_back( mtypes.at(i).toStdString());
	}
	m_collectionUpdater.setFileTypes( types );

	if(settings.contains("statedb") ) {
			m_db_filename = settings.value("statedb").toString();
	}
	else {
		// mediadir was not mentioned in config file, make a dummy entry for it so the user can change that.
		m_db_filename = "./statedb.sqlite";
		settings.setValue("statedb", m_db_filename);
	}
}

void writeSettings() {

}
