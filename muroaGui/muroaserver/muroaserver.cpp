#include "muroaserver.h"

#include "CSession.h"

#include <QMessageBox>
#include <QFile>

muroaserver::muroaserver(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

	m_session = new CSession();

	statusBar()->addWidget(&m_connection_status_label);

	connect(&m_connection, SIGNAL(connectionStatusChanged(QString)), this, SLOT(connectionStatusChanged(QString)));
	readCollectionFile("test/collection1.txt");
	readCollectionFile("test/collection2.txt");
	readCollectionFile("test/collection3.txt");
	readCollectionFile("test/collection4.txt");
	readCollectionFile("test/collection5.txt");
	readCollectionFile("test/collection6.txt");
	readCollectionFile("test/collection7.txt");
	m_connection.setSessionPtr(m_session);
	//m_connection.setCollection(&m_collection);
}

muroaserver::~muroaserver()
{
	delete m_session;
}


void muroaserver::connectionStatusChanged(QString status)
{
	m_connection_status_label.setText(status);
}

void muroaserver::readCollectionFile(QString filename)
{
	QFile collectionFile(filename);
	QString line;

	if (!collectionFile.open(QIODevice::ReadOnly)) // Open the file
	{
        QMessageBox::warning(this, "Load Collection", QString("Can not load collection from file %1").arg(filename));
	}

	QTextStream stream( &collectionFile ); // Set the stream to read from myFile

	QString text = stream.readAll();

	m_session->addCollectionRev( QString( text ) );
//	do
//	{
//		line = stream.readLine();
//		if(line.isNull()) break;
//
//		m_collection.parseLine(line);
//
//	}
//	while(!line.isNull());
}
