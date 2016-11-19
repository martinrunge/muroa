/*
 Copyright (c) 2002-2014 "Martin Runge"

 CmdStreamBase.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef CMDSTREAMBASE_H_
#define CMDSTREAMBASE_H_

#include <cstdint>

#include <typeinfo>
#include <map>
#include <chrono>

namespace muroa {

class CmdStreamBase {
public:
	virtual ~CmdStreamBase();
	uint32_t getID() const { return m_cmd_id; };
	uint32_t m_cmd_id;

protected:
	CmdStreamBase();

	static std::map<std::type_info, int> m_visitor_map;
	static std::map<std::chrono::time_point<std::chrono::steady_clock>, CmdStreamBase*> m_timeouts;


};

} /* namespace muroa */

#endif /* CMDSTREAMBASE_H_ */
