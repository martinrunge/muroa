/*
 * CCmdBase.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#include "Cmd.h"

using namespace std;

namespace muroa {

int Cmd::m_timeout_in_ms = 100;
long long Cmd::m_first_free_id = 0;


Cmd::Cmd(type_t type) : m_type(type), m_id(m_first_free_id++) {

}

Cmd::~Cmd() {
	/// TODO stop any timers
}

void Cmd::timeout() {

}

} /* namespace muroa */
