/*
 * CSessionAdminDlg.cpp
 *
 *  Created on: 30. Sep 2012
 *      Author: martinrunge
 */

#include "CSessionAdminDlg.h"
#include "CRenderClientsDiffBuilder.h"
#include "CRenderClientsListModel.h"
#include "CConnection.h"
#include <CRootItem.h>
#include <CSession.h>
#include <CCategoryItem.h>

using namespace muroa;

CSessionAdminDlg::CSessionAdminDlg(const CSession* const session,
                                   QWidget* parent):
		                        		                m_sessionState(session->getSessionState()),
		                        		                m_session(session),
		                        		                m_connection(session->getConnection())
{
	ui.setupUi(this);
	// setRejoinState( m_settings.value("rejoin").toBool() );

	m_availRenderClientsModel = new CRenderClientsListModel(m_sessionState, m_session);
	m_availRenderClientsModel->setBase("/RenderClients");
	m_availRenderClientsModel->setRole(E_AVAIL_RENDER_CLIENT);
	m_sessionState->connectItemModel(m_availRenderClientsModel);

	m_ownRenderClientsModel = new CRenderClientsListModel(m_sessionState, m_session);
	m_ownRenderClientsModel->setBase("/RenderClients");
	m_ownRenderClientsModel->setRole(E_OWN_RENDER_CLIENT);
	m_sessionState->connectItemModel(m_ownRenderClientsModel);

	m_rcDiffBuilder = new CRenderClientsDiffBuilder(m_sessionState);

	ui.availRenderersListView->setRole(E_AVAIL_RENDER_CLIENT);
  	ui.availRenderersListView->setModel(m_availRenderClientsModel);
  	ui.availRenderersListView->setDiffBuilder( m_rcDiffBuilder );

  	ui.ownRenderersListView->setRole(E_OWN_RENDER_CLIENT);
  	ui.ownRenderersListView->setModel(m_ownRenderClientsModel);
  	ui.ownRenderersListView->setDiffBuilder( m_rcDiffBuilder );


  	connect(m_rcDiffBuilder, SIGNAL(sendCommand(CmdBase*)), m_connection, SLOT(sendCommand(CmdBase*)));
}

CSessionAdminDlg::~CSessionAdminDlg() {
	ui.ownRenderersListView->setModel(0);
	ui.availRenderersListView->setModel(0);

	delete m_rcDiffBuilder;
	// model not connected to view any more. Hopefully its save to delete it now.
	delete m_ownRenderClientsModel;
	delete m_availRenderClientsModel;
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
