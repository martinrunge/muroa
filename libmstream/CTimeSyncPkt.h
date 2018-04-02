/*
 Copyright (c) 2002-2014 "Martin Runge"

 CTimeSyncPkt.h is part of muroa, the  Multi Room Audio Player [http://www.muroa.org]

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

#ifndef LIBMSTREAM_CTIMESYNCPKT_H_
#define LIBMSTREAM_CTIMESYNCPKT_H_

#include <CTime.h>
#include <cstdint>
#include "Exceptions.h"

namespace muroa {

typedef struct buffer {
	char* buffer;
	size_t size;
} buffer_t;

class CTimeSyncPkt {
public:
	CTimeSyncPkt();
	CTimeSyncPkt(buffer_t buf);
	CTimeSyncPkt(const char* buf, size_t len);

	virtual ~CTimeSyncPkt();

	CDuration getTransitionTime();

	buffer_t serialize();
	void deserialize(buffer_t buf);                    // might throw CDeserialisationException
	void deserialize(const char* buf, size_t size);    // might throw CDeserialisationException

	const CTime& getT1() const {
		return m_t1;
	}

	void setT1(const CTime& t1) {
		m_t1 = t1;
	}

	const CTime& getT1Tick() const {
		return m_t1_tick;
	}

	void setT1Tick(const CTime& t1Tick) {
		m_t1_tick = t1Tick;
	}

	const CTime& getT2() const {
		return m_t2;
	}

	void setT2(const CTime& t2) {
		m_t2 = t2;
	}

	const CTime& getT2Tick() const {
		return m_t2_tick;
	}

	void setT2Tick(const CTime& t2Tick) {
		m_t2_tick = t2Tick;
	}

private:
	CTime m_t1;
	CTime m_t1_tick;

	CTime m_t2;
	CTime m_t2_tick;

	union packaging {
		struct _times {
			uint64_t t1;
			uint64_t t1_tick;
			uint64_t t2;
			uint64_t t2_tick;
		} times;
		char buffer[4 * sizeof(int64_t)];
	};

	void deserialize(packaging p);         // might throw CDeserialisationException

};

} /* namespace muroa */

#endif /* LIBMSTREAM_CTIMESYNCPKT_H_ */
