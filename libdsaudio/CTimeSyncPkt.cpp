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

}


CTimeSyncPkt::~CTimeSyncPkt() {
}

CDuration CTimeSyncPkt::getTransitionTime() {
	CDuration d1 = m_t1_tick - m_t1;
	CDuration d2 = m_t2_tick - m_t2;

	return (d1 + d2) / 2;
}

buffer_t CTimeSyncPkt::serialize() {
	packaging p;
	p.times.t1      = m_t1.serialize();
	p.times.t1_tick = m_t1_tick.serialize();
	p.times.t2      = m_t2.serialize();
	p.times.t2_tick = m_t2_tick.serialize();

	return buffer_t{p.buffer, sizeof(p.buffer)};
}

void CTimeSyncPkt::deserialize(buffer_t buf) throw(CDeserialisationException) {
	if( buf.size != sizeof(packaging) ) {
		throw CDeserialisationException("CTimeSyncPkt::deserialize");
	}

	packaging p;
	memcpy(p.buffer, buf.buffer, buf.size);
	deserialize(p);
}

void CTimeSyncPkt::deserialize(const char* buf, size_t size) throw(CDeserialisationException) {
	if( size != sizeof(packaging) ) {
		throw CDeserialisationException("CTimeSyncPkt::deserialize");
	}

	packaging p;
	memcpy(p.buffer, buf, size);
	deserialize(p);
}

void CTimeSyncPkt::deserialize(packaging p) throw(CDeserialisationException) {
	m_t1.deserialize(p.times.t1);
	m_t1_tick.deserialize(p.times.t1_tick);
	m_t2.deserialize(p.times.t2);
	m_t2_tick.deserialize(p.times.t2_tick);
}

} /* namespace muroa */
