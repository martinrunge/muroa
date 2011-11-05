/*
 * CCmdBase.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#include "CCmdBase.h"

using namespace std;

namespace muroa {

int CCmdBase::m_timeout_in_ms = 100;
long long CCmdBase::m_first_free_id = 0;


CCmdBase::CCmdBase(	enum cmd_type type) : m_type(type), m_id(m_first_free_id++) {

}

CCmdBase::~CCmdBase() {
	/// TODO stop any timers
}

void CCmdBase::timeout() {

}

} /* namespace muroa */
