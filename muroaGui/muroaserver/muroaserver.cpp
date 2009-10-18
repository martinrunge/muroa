#include "muroaserver.h"

#include <QMessageBox>
#include <QFile>

muroaserver::muroaserver(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

	statusBar()->addWidget(&m_connection_status_label);

	connect(&m_connection, SIGNAL(connectionStatusChanged(QString)), this, SLOT(connectionStatusChanged(QString)));
	readCollectionFile("/home/martin/collection.txt");
	m_connection.setCollection(&m_collection);
}

muroaserver::~muroaserver()
{

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

	do
	{
		line = stream.readLine();
		if(line.isNull()) break;

		m_collection.parseLine(line);

	}
	while(!line.isNull());
}
