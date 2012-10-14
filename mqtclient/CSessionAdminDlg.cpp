/*
 * CSessionAdminDlg.cpp
 *
 *  Created on: 30. Sep 2012
 *      Author: martinrunge
 */

#include "CSessionAdminDlg.h"
#include <CRootItem.h>
#include <CCategoryItem.h>
#include <CMuroaListModel.h>

using namespace muroa;

CSessionAdminDlg::CSessionAdminDlg(CRootItem* sessionState ,QWidget* parent): m_sessionState(sessionState)
{
	ui.setupUi(this);
	// setRejoinState( m_settings.value("rejoin").toBool() );

	m_availClientsModel = new CMuroaListModel();
	CCategoryItem* availbase = m_sessionState->getCategoryPtr("/AvailableStreamClients");
	if(availbase == 0) {
		// not yet there, create it
		availbase = m_sessionState->mkPath("/AvailableStreamClients");
	}
	m_availClientsModel->setBase(availbase);
	m_sessionState->connectItemModel(m_availClientsModel);

//	m_ownClientsModel = new CMuroaListModel();
//	CCategoryItem* ownbase = m_sessionState->getCategoryPtr("/ownStreamClients");
//	if(ownbase == 0) {
//		// not yet there, create it
//		ownbase = m_sessionState->mkPath("/ownStreamClients");
//	}
//  	m_ownClientsModel->setBase(ownbase);
//	m_sessionState->connectItemModel(m_ownClientsModel);

  	// ui.ownRenderersListView->setModel(m_ownClientsModel);
  	ui.availRenderersListView->setModel(m_availClientsModel);
}

CSessionAdminDlg::~CSessionAdminDlg() {
	// ui.ownRenderersListView->setModel(0);
	ui.availRenderersListView->setModel(0);
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
