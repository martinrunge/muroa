/*
 * CPreferencesDlg.cpp
 *
 *  Created on: 4 Jul 2010
 *      Author: martin
 */

#include "CPreferencesDlg.h"

CPreferencesDlg::CPreferencesDlg(QWidget* parent) {
	ui.setupUi(this);
	setRejoinState( m_settings.value("rejoin").toBool() );
}

CPreferencesDlg::~CPreferencesDlg() {
}


void CPreferencesDlg::accept()
{
	m_settings.setValue("rejoin", getRejoinState());
	QDialog::accept();
}

void CPreferencesDlg::reject()
{
	QDialog::reject();
}
