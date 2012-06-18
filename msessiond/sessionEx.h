/*
 * Exceptions.h
 *
 *  Created on: 11 Sep 2010
 *      Author: martin
 */

#ifndef SESSION_EX_H_
#define SESSION_EX_H_

#include <exception>
#include <string>

// namespace muroa {

class CException: public std::exception {
public:
	CException(std::string reason) throw ();
	virtual ~CException() throw ();

	const char* what() const throw ();
	const std::string reason() const throw ();
private:
	std::string m_reason;
};

class CApiMisuseException: public std::exception {
public:
	CApiMisuseException(std::string reason) throw ();
	virtual ~CApiMisuseException() throw ();

	const char* what() const throw ();
	const std::string reason() const throw ();
private:
	std::string m_reason;
};


class NeedMoreDataException: public std::exception {
public:
	NeedMoreDataException(std::string reason, int code = 0) throw ();
	virtual ~NeedMoreDataException() throw ();

	const char* what() const throw ();
	const std::string reason() const throw ();
	const int code() const throw ();

private:
	std::string m_reason;
	int m_code;
};

// }

#endif /* SESSION_EX */
