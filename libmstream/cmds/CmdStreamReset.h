/*
 Copyright (c) 2002-2014 "Martin Runge"

 CmdStreamReset.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef CMDSTREAMRESET_H_
#define CMDSTREAMRESET_H_

#include "CmdStreamBase.h"

#include <cstdint>

class CRTPPacket;

namespace muroa {

class CmdStreamReset :public CmdStreamBase {
public:

	CmdStreamReset( uint32_t m_oldSessionID = 0, uint32_t m_oldStreamID = 0, uint32_t m_newSessionID = 0, uint32_t m_newStreamID = 0 );
	CmdStreamReset(	CRTPPacket* resetPkt);
	virtual ~CmdStreamReset();

	CRTPPacket* toRTP();

	uint32_t getNewSessionId() const {
		return m_newSessionID;
	}

	void setNewSessionId(uint32_t newSessionId) {
		m_newSessionID = newSessionId;
	}

	uint32_t getNewStreamId() const {
		return m_newStreamID;
	}

	void setNewStreamId(uint32_t newStreamId) {
		m_newStreamID = newStreamId;
	}

	uint32_t getOldSessionId() const {
		return m_oldSessionID;
	}

	void setOldSessionId(uint32_t oldSessionId) {
		m_oldSessionID = oldSessionId;
	}

	uint32_t getOldStreamId() const {
		return m_oldStreamID;
	}

	void setOldStreamId(uint32_t oldStreamId) {
		m_oldStreamID = oldStreamId;
	}

private:
	uint32_t m_oldStreamID;
	uint32_t m_oldSessionID;

	uint32_t m_newStreamID;
	uint32_t m_newSessionID;


	typedef struct serialisation_vars{
		uint32_t oldStreamID;
		uint32_t oldSessionID;

		uint32_t newStreamID;
		uint32_t newSessionID;
	}serialisation_vars_t;

	typedef union serialisation_buffer{
	  serialisation_vars_t serialisation_vars;
	  char raw_buffer[sizeof(serialisation_vars_t)];
	} serialisation_buffer_t;

	// serialisation_buffer_t m_serialisation_buffer;
};

} /* namespace muroa */

#endif /* CMDSTREAMRESET_H_ */
