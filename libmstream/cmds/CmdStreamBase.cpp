/*
 Copyright (c) 2002-2014 "Martin Runge"

 CmdStreamBase.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CmdStreamBase.h"

namespace muroa {

std::map<std::type_info, int> CmdStreamBase::m_visitor_map;
std::map<std::chrono::time_point<std::chrono::steady_clock>, CmdStreamBase*> CmdStreamBase::m_timeouts;

CmdStreamBase::CmdStreamBase() {
	// TODO Auto-generated constructor stub

}

CmdStreamBase::~CmdStreamBase() {
	// TODO Auto-generated destructor stub
}

} /* namespace muroa */
