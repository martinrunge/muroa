/*
 Copyright (c) 2002-2014 "Martin Runge"

 CTimeSyncPkt.cpp is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#include "CTimeSyncPkt.h"

#include <asm/byteorder.h>

namespace muroa {

CTimeSyncPkt::CTimeSyncPkt() {
}

CTimeSyncPkt::CTimeSyncPkt(buffer_t buf) {
	deserialize(buf);
}

CTimeSyncPkt::CTimeSyncPkt(const char* buf, size_t len) {
	deserialize(buf, len);
}


CTimeSyncPkt::~CTimeSyncPkt() {
}

const CDuration CTimeSyncPkt::getRoundTripDelay() {
	const CDuration d1 = m_t4 - m_t1;
	const CDuration d2 = m_t3 - m_t2;

	return d1 - d2;
}

const CDuration CTimeSyncPkt::getClockOffset() {
	// theta = T(B) - T(A) = 1/2 * [(T2-T1) + (T3-T4)]

	const CDuration d1 = m_t2 - m_t1;
	const CDuration d2 = m_t3 - m_t4;

	const CDuration theta = (d1 + d2) * 0.5;
	return theta;
}

buffer_t CTimeSyncPkt::serialize() {
	m_p.times.t1 = m_t1.serialize();
	m_p.times.t2 = m_t2.serialize();
	m_p.times.t3 = m_t3.serialize();
	m_p.times.t4 = m_t4.serialize();

	return buffer_t{m_p.buffer, sizeof(m_p.buffer)};
}

void CTimeSyncPkt::deserialize(buffer_t buf) {    // might throw CDeserialisationException
	if( buf.size != sizeof(packaging) ) {
		throw CDeserialisationException("CTimeSyncPkt::deserialize");
	}

	packaging p;
	memcpy(p.buffer, buf.buffer, buf.size);
	deserialize(p);
}

void CTimeSyncPkt::deserialize(const char* buf, size_t size) {   // might throw CDeserialisationException
	if( size != sizeof(packaging) ) {
		throw CDeserialisationException("CTimeSyncPkt::deserialize");
	}

	packaging p;
	memcpy(p.buffer, buf, size);
	deserialize(p);
}

void CTimeSyncPkt::deserialize(packaging p) {     // might throw CDeserialisationException
	m_t1.deserialize(p.times.t1);
	m_t2.deserialize(p.times.t2);
	m_t3.deserialize(p.times.t3);
	m_t4.deserialize(p.times.t4);
}

} /* namespace muroa */
