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
#include <asm/byteorder.h>
#include "Exceptions.h"

inline struct timespec i64_to_timespec(const int64_t& i64time) {
	struct timespec ts;
	ts.tv_sec = i64time / 1000000000LL;
	ts.tv_nsec = i64time - ts.tv_sec;
	return ts;
}

inline int64_t timespec_to_i64(const struct timespec& ts) {
	int64_t retval = ts.tv_nsec;
	retval += 1000000000LL * ts.tv_sec;
	return retval;
}

class CDuration {
public:
	CDuration(int hour, int min, int sec, int64_t nanosec ) {
		m_time_in_ns = 1000000000LL * 3600 * hour
				     + 1000000000LL * 60 * min
				     + 1000000000LL * sec
				     + nanosec;
	}
	CDuration(int64_t nanosec = 0) {
		m_time_in_ns = nanosec;
	}

	CDuration(double sec) {
		m_time_in_ns = 1000000000LL * sec;
	}

	virtual ~CDuration() {};

	bool isNull() { return (m_time_in_ns == 0); };

	CDuration sleep() throw(std::exception) {
		struct timespec ts = i64_to_timespec(m_time_in_ns);
		struct timespec remain;
		errno = 0;
		int rc = nanosleep(&ts, &remain);
		if(rc == -1) {
			if(errno == EINTR) {
				return CDuration( timespec_to_i64(remain) );
			}
			else {
				std::stringstream ss;
				ss << "nanosleep returned with error (errno: " << errno << ", '" << strerror(errno) <<"')";
				throw CException(ss.str());
			}
		}
		else {
			return CDuration((int64_t)0LL);
		}
	}

	int64_t ns() {
		return m_time_in_ns;
	}

	int64_t us() {
		return m_time_in_ns / 1000;
	}

	int64_t ms() {
		return m_time_in_ns / 1000000;
	}

	double sec() {
		double factor = 1.0/1000000000LL;
		return factor * m_time_in_ns;
	}

	bool operator==( const CDuration& rhs ) const {
		return ( m_time_in_ns == rhs.m_time_in_ns );
	}

	bool operator!=( const CDuration& rhs ) const {
	    return ( m_time_in_ns != rhs.m_time_in_ns );
	}

	CDuration operator+( const CDuration& rhs ) const {
		return CDuration( m_time_in_ns + rhs.m_time_in_ns );
	}

	CDuration operator-( const CDuration& rhs ) const {
		return CDuration( m_time_in_ns - rhs.m_time_in_ns );
	}

	CDuration operator*( const int& rhs ) const {
		return CDuration( m_time_in_ns * rhs );
	}

	CDuration operator/( const int& rhs ) const {
		return CDuration( m_time_in_ns / rhs );
	}

	CDuration operator*( const double& rhs ) const {
		return CDuration( m_time_in_ns * rhs );
	}

	CDuration operator/( const double& rhs ) const {
		return CDuration( m_time_in_ns / rhs );
	}

	int64_t serialize() {
		return __cpu_to_le64(m_time_in_ns);
	}

	void deserialize(int64_t time_in_le) {
		m_time_in_ns = __le64_to_cpu(time_in_le);
	}

private:
	int64_t m_time_in_ns;

	friend class CTime;
};



class CTime {
public:
	CTime() {
		;
	}

	CTime( const int64_t& nanosec ) {
		m_time_in_ns = nanosec;
	}

	CTime( const struct timespec& ts ) {
		m_time_in_ns = timespec_to_i64(ts);
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

	int64_t serialize() {
		return __cpu_to_le64(m_time_in_ns);
	}

	void deserialize(int64_t time_in_le) {
		m_time_in_ns = __le64_to_cpu(time_in_le);
	}

private:
	int64_t m_time_in_ns;
};



#endif /* CTIME_H_ */
