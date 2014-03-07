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

namespace muroa {

class CmdStreamBase {
public:
	enum cmd_stream_t {
		E_RESET_STREAM,
		E_UNKNOWN
	};

	virtual ~CmdStreamBase();

	enum cmd_stream_t type() { return m_type; };

protected:
	CmdStreamBase(enum cmd_stream_t type);
	const enum cmd_stream_t m_type;
};

} /* namespace muroa */

#endif /* CMDSTREAMBASE_H_ */
