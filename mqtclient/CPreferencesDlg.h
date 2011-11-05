/*
 * CPreferencesDlg.h
 *
 *  Created on: 4 Jul 2010
 *      Author: martin
 */

#ifndef CPREFERENCESDLG_H_
#define CPREFERENCESDLG_H_

#include <QDialog>
#include <QSettings>
#include "ui_PreferencesDlg.h"

class CPreferencesDlg  : public QDialog
{
	Q_OBJECT;
public:
	CPreferencesDlg(QWidget* parent = 0);
	virtual ~CPreferencesDlg();

public slots:
	void accept();
    void reject();

	inline bool getRejoinState() {
		if( Qt::Checked == ui.rejoinCheckBox->checkState()) {
			return true;
		}
		else {
			return false;
		}
	};

	inline void setRejoinState(bool rejoin) {
		if( rejoin ) {
			ui.rejoinCheckBox->setCheckState(Qt::Checked);
		}
		else {
			ui.rejoinCheckBox->setCheckState(Qt::Unchecked);
		}
	};


private:
    Ui::PreferencesDlg ui;
  	QSettings m_settings;


};

#endif /* CPREFERENCESDLG_H_ */
