/*
 * CTime.h
 *
 *  Created on: 17 Sep 2010
 *      Author: martin
 */

#ifndef CTIME_H_
#define CTIME_H_

#include <sstream>
#include <time.h>
#include <string.h>
#include "Exceptions.h"

inline struct timespec ll_to_timespec(const long long& lltime) {
	struct timespec ts;
	ts.tv_sec = lltime / 1000000000LL;
	ts.tv_nsec = lltime - ts.tv_sec;
	return ts;
}

inline long long timespec_to_ll(const struct timespec& ts) {
	long long retval = ts.tv_nsec;
	retval += 1000000000LL * ts.tv_sec;
	return retval;
}

class CDuration {
public:
	CDuration(int hour, int min, int sec, int nanosec ) {
		m_time_in_ns = 1000000000LL * 3600 * hour
				     + 1000000000LL * 60 * min
				     + 1000000000LL * sec
				     + nanosec;
	}
	CDuration(long long nanosec = 0) {
		m_time_in_ns = nanosec;
	}

	CDuration(double sec) {
		m_time_in_ns = 1000000000LL * sec;
	}

	virtual ~CDuration() {};

	bool isNull() { return (m_time_in_ns == 0); };

	CDuration sleep() throw(CException) {
		struct timespec ts = ll_to_timespec(m_time_in_ns);
		struct timespec remain;
		errno = 0;
		int rc = nanosleep(&ts, &remain);
		if(rc == -1) {
			if(errno == EINTR) {
				return CDuration( timespec_to_ll(remain) );
			}
			else {
				std::stringstream ss;
				ss << "nanosleep returned with error (errno: " << errno << ", '" << strerror(errno) <<"')";
				throw CException(ss.str());
			}
		}
		else {
			return CDuration(0LL);
		}
	}

	long long ns() {
		return m_time_in_ns;
	}

	long long us() {
		return m_time_in_ns / 1000;
	}

	long long ms() {
		return m_time_in_ns / 1000000;
	}

	double sec() {
		double factor = 1.0/1000000000LL;
		return factor * m_time_in_ns;
	}

private:
	long long m_time_in_ns;

	friend class CTime;
};



class CTime {
public:
	CTime() {
		;
	}

	CTime( const long long& nanosec ) {
		m_time_in_ns = nanosec;
	}

	CTime( const struct timespec& ts ) {
		m_time_in_ns = timespec_to_ll(ts);
	}

	bool operator==( const CTime& rhs ) const {
		return ( m_time_in_ns == rhs.m_time_in_ns );
	}

	bool operator!=( const CTime& rhs ) const {
	    return ( m_time_in_ns != rhs.m_time_in_ns );
	}

	CTime operator+( const CDuration& rhs ) const {
		return CTime( m_time_in_ns + rhs.m_time_in_ns );
	}

	CTime operator-( const CDuration& rhs ) const {
		return CTime( m_time_in_ns - rhs.m_time_in_ns );
	}

	CDuration operator-( const CTime& rhs ) const {
		return CDuration( m_time_in_ns - rhs.m_time_in_ns );
	}

	virtual ~CTime() {};
	static CTime now() throw(CException) {
		struct timespec ts;
		int retval = clock_gettime( CLOCK_MONOTONIC, &ts );
		if(retval != 0) {
			std::stringstream ss;
			ss << "clock_gettime failed with error (errno: " << errno << ", '" << strerror(errno) << "')";
			throw CException(ss.str());
		}
		else {
			return CTime(ts);
		}
	}

private:
	long long m_time_in_ns;
};



#endif /* CTIME_H_ */
