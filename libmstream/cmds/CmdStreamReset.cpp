/*
 Copyright (c) 2002-2014 "Martin Runge"

 CmdStreamReset.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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


#include "CmdStreamReset.h"

#include "crtppacket.h"
#include <arpa/inet.h>
#include <string.h>


namespace muroa {


CmdStreamReset::CmdStreamReset(	uint32_t oldSessionID, uint32_t oldStreamID,
		                        uint32_t newSessionID, uint32_t newStreamID)
               : CmdStreamBase()
{
	m_oldSessionID = oldSessionID;
	m_oldStreamID = oldStreamID;
	m_newSessionID = newSessionID;
	m_newStreamID = newStreamID;
}

CmdStreamReset::CmdStreamReset(	CRTPPacket* resetPkt ) : CmdStreamBase() {
	serialisation_buffer_t* serbuf = reinterpret_cast<serialisation_buffer_t*>(resetPkt->payloadBufferPtr());

	m_oldSessionID = ntohl( serbuf->serialisation_vars.oldSessionID );
	m_oldStreamID = ntohl( serbuf->serialisation_vars.oldStreamID );

	m_newSessionID = ntohl( serbuf->serialisation_vars.newSessionID );
	m_newStreamID = ntohl( serbuf->serialisation_vars.newStreamID );
}

CRTPPacket* CmdStreamReset::toRTP() {
	CRTPPacket* pkt = new CRTPPacket(m_oldSessionID, m_oldStreamID, sizeof(serialisation_buffer_t), true);
	serialisation_buffer_t* serbuf = reinterpret_cast<serialisation_buffer_t*>(pkt->payloadBufferPtr());

	serbuf->serialisation_vars.oldSessionID = htonl(m_oldSessionID);
	serbuf->serialisation_vars.oldStreamID = htonl(m_oldStreamID);

	serbuf->serialisation_vars.newSessionID = htonl(m_newSessionID);
	serbuf->serialisation_vars.newStreamID = htonl(m_newStreamID);

	pkt->usedPayloadBufferSize(sizeof(serialisation_buffer_t));
	pkt->payloadType(PAYLOAD_RESET_STREAM);

	return pkt;
}


CmdStreamReset::~CmdStreamReset() {
	// TODO Auto-generated destructor stub
}

} /* namespace muroa */
