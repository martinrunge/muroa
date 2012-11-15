/*
 * CSessionAdminDlg.cpp
 *
 *  Created on: 30. Sep 2012
 *      Author: martinrunge
 */

#include "CSessionAdminDlg.h"
#include "CRenderClientsDiffBuilder.h"
#include "CConnection.h"
#include <CRootItem.h>
#include <CCategoryItem.h>
#include <CMuroaListModel.h>

using namespace muroa;

CSessionAdminDlg::CSessionAdminDlg(CRootItem* sessionState,
                                   const CConnection* connection,
		                           QWidget* parent):
		                        		                m_sessionState(sessionState),
		                        		                m_connection(connection)
{
	ui.setupUi(this);
	// setRejoinState( m_settings.value("rejoin").toBool() );

	m_availClientsModel = new CMuroaListModel(m_sessionState);
	m_availClientsModel->setBase("/AvailableStreamClients");
	m_sessionState->connectItemModel(m_availClientsModel);

	m_ownClientsModel = new CMuroaListModel(m_sessionState);
	m_ownClientsModel->setBase("/OwnStreamClients");
	m_sessionState->connectItemModel(m_ownClientsModel);

	m_rcDiffBuilder = new CRenderClientsDiffBuilder(sessionState);

	ui.ownRenderersListView->setRole(E_OWN_RENDER_CLIENT);
  	ui.ownRenderersListView->setModel(m_ownClientsModel);
  	ui.ownRenderersListView->setDiffBuilder( m_rcDiffBuilder );

	ui.availRenderersListView->setRole(E_AVAIL_RENDER_CLIENT);
  	ui.availRenderersListView->setModel(m_availClientsModel);
  	ui.availRenderersListView->setDiffBuilder( m_rcDiffBuilder );

  	connect(m_rcDiffBuilder, SIGNAL(sendCommand(CmdBase*)), m_connection, SLOT(sendCommand(CmdBase*)));
}

CSessionAdminDlg::~CSessionAdminDlg() {
	ui.ownRenderersListView->setModel(0);
	ui.availRenderersListView->setModel(0);

	delete m_rcDiffBuilder;
	// model not connected to view any more. Hopefully its save to delete it now.
	delete m_ownClientsModel;
	delete m_availClientsModel;
}


void CSessionAdminDlg::accept()
{
	// m_settings.setValue("rejoin", getRejoinState());
	QDialog::accept();
}

void CSessionAdminDlg::reject()
{
	QDialog::reject();
}
