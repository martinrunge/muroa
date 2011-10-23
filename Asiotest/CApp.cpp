/*
 * CApp.cpp
 *
 *  Created on: 23 Oct 2011
 *      Author: martin
 */

#include "CApp.h"

#include <iostream>

namespace muroa {

CApp* CApp::m_inst_ptr = 0;
std::mutex CApp::m_mutex;

CSettings* CApp::getSettingsPtr(void) {
	return m_settings_ptr;
}

void CApp::setSettingsPtr(CSettings* settings_ptr) {
	m_settings_ptr = settings_ptr;
}

CApp::CApp() {}
CApp::~CApp() {}

CApp* CApp::getInstPtr() {
	std::lock_guard<std::mutex> lk(m_mutex);
	if( m_inst_ptr == 0) {
		m_inst_ptr = new CApp();
	}
	return m_inst_ptr;
}

void CApp::serviceChanged() {
	std::cerr << "CApp::serviceChanged()" << std::endl;
}


} /* namespace muroa */
