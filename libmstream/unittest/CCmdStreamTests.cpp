/*
 Copyright (c) 2002-2014 "Martin Runge"

 CCmdStreamTests.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

 Muroa is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include "CCmdStreamTests.h"

#include "cmds/CmdStreamConnect.h"

#include <string>

using namespace std;

namespace muroa {

CPPUNIT_TEST_SUITE_REGISTRATION( CCmdStreamTests );

CCmdStreamTests::CCmdStreamTests() {
	// TODO Auto-generated constructor stub

}

CCmdStreamTests::~CCmdStreamTests() {
	// TODO Auto-generated destructor stub
}

void CCmdStreamTests::setUp() {
}

void CCmdStreamTests::tearDown() {
}

void CCmdStreamTests::serializeStreamReset() {
}

void CCmdStreamTests::serializeStreamConnectIPv4() {
	CmdStreamConnect cmd_conn("192.168.1.1", 5566, "239.1.1.1");
	string res = cmd_conn.serialize();

}

void CCmdStreamTests::serializeStreamConnectIPv6() {
}

void CCmdStreamTests::deserializeStreamConnectIPv4() {
}

void CCmdStreamTests::deserializeStreamConnectIPv6() {
}


} /* namespace muroa */
