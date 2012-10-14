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

class CMuroaListModel;

namespace muroa {
	class CRootItem;
}

class CSessionAdminDlg  : public QDialog
{
	Q_OBJECT;
public:
	CSessionAdminDlg(muroa::CRootItem* sessionState, QWidget* parent = 0);
	virtual ~CSessionAdminDlg();

public slots:
	void accept();
    void reject();

private:
    Ui::SessionAdminDlg ui;
  	QSettings m_settings;

  	muroa::CRootItem* m_sessionState;

  	CMuroaListModel *m_availClientsModel;
  	CMuroaListModel *m_ownClientsModel;
};

#endif /* CSESSIONADMINDLG_H_ */
