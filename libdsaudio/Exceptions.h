/*
 * Exceptions.h
 *
 *  Created on: 11 Sep 2010
 *      Author: martin
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <string>

class CException: public std::exception {
public:
	CException(std::string reason) throw () { m_reason = reason; };
	virtual ~CException() throw () {};

	const char* what() const throw () { return m_reason.c_str(); };
	const std::string reason() const throw () { return m_reason; };
private:
	std::string m_reason;
};

class CDeserialisationException: public std::exception {
public:
	CDeserialisationException(std::string reason) throw () { m_reason = reason; };
	virtual ~CDeserialisationException() throw () {};

	const char* what() const throw () { return m_reason.c_str(); };
	const std::string reason() const throw () { return m_reason; };
private:
	std::string m_reason;
};


#endif /* EXCEPTIONS_H_ */
