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
	CException(std::string reason) throw();
	virtual ~CException() throw();

	const char* what() const throw();
	const std::string reason() const throw();
private:
	std::string m_reason;
};


class CApiMisuseException: public std::exception {
public:
	CApiMisuseException(std::string reason) throw();
	virtual ~CApiMisuseException() throw();

	const char* what() const throw();
	const std::string reason() const throw();
private:
	std::string m_reason;
};

class InvalidMsgException: public std::exception {
public:
	InvalidMsgException(std::string reason) throw();
	virtual ~InvalidMsgException() throw();

	const char* what() const throw();
	const std::string reason() const throw();
private:
	std::string m_reason;
};



#endif /* EXCEPTIONS_H_ */
