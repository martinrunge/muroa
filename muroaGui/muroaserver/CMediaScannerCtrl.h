/*
 * CMediaScannerCtrl.h
 *
 *  Created on: 24 Oct 2010
 *      Author: martin
 */

#ifndef CMEDIASCANNERCTRL_H_
#define CMEDIASCANNERCTRL_H_

#include "CSubProcess.h"

class CMediaScannerCtrl {
public:
	CMediaScannerCtrl();
	virtual ~CMediaScannerCtrl();

	void start();

private:
	CSubProcess m_subProcess;
	int m_socket;
};

#endif /* CMEDIASCANNERCTRL_H_ */
