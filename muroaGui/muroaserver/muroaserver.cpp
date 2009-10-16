#include "muroaserver.h"

muroaserver::muroaserver(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

	statusBar()->addWidget(&m_connection_status_label);

	connect(&m_connection, SIGNAL(connectionStatusChanged(QString)), this, SLOT(connectionStatusChanged(QString)));
}

muroaserver::~muroaserver()
{

}


void muroaserver::connectionStatusChanged(QString status)
{
	m_connection_status_label.setText(status);
}
