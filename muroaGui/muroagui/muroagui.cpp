#include "muroagui.h"

muroagui::muroagui(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openConnection()));
    connect(ui.actionClose, SIGNAL(triggered()), &m_connection, SLOT(close()));

	statusBar()->addWidget(&m_connection_status_label);

	connect(&m_connection, SIGNAL(connectionStatusChanged(QString)), this, SLOT(connectionStatusChanged(QString)));


}

muroagui::~muroagui()
{

}



void muroagui::openConnection()
{
    m_connection.open("localhost", 2678);
}


void muroagui::connectionStatusChanged(QString status)
{
	m_connection_status_label.setText(status);
}
