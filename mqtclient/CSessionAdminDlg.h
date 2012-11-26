/*
 * CSessionAdminDlg.h
 *
 *  Created on: 30. Sep 2012
 *      Author: martinrunge
 */

#ifndef CSESSIONADMINDLG_H_
#define CSESSIONADMINDLG_H_

#include <QDialog>
#include <QSettings>
#include "ui_SessionAdminDlg.h"


class CRenderClientsDiffBuilder;
class CRenderClientsListModel;

namespace muroa {
	class CRootItem;
}
class CConnection;
class CSession;

class CSessionAdminDlg  : public QDialog
{
	Q_OBJECT;
public:
	CSessionAdminDlg(const CSession* const session,
			         QWidget* parent = 0);
	virtual ~CSessionAdminDlg();

public slots:
	void accept();
    void reject();

private:
    Ui::SessionAdminDlg ui;
  	QSettings m_settings;

  	CRenderClientsDiffBuilder* m_rcDiffBuilder;

  	muroa::CRootItem* m_sessionState;
  	const CSession* const m_session;
  	const CConnection* const m_connection;

  	CRenderClientsListModel *m_availRenderClientsModel;
  	CRenderClientsListModel *m_ownRenderClientsModel;
};

#endif /* CSESSIONADMINDLG_H_ */
