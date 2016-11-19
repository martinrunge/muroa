/*
 * Exceptions.h
 *
 *  Created on: 11 Sep 2010
 *      Author: martin
 */

#ifndef LIBMSTREAM_EXCEPTIONS_H_
#define LIBMSTREAM_EXCEPTIONS_H_

#include <exception>
#include <string>
#include <boost/asio.hpp>

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

class CUnknownServiceNameException: public std::exception {
public:
	CUnknownServiceNameException(std::string unknown_service_name) throw () { m_unknown_service_name = unknown_service_name; };
	virtual ~CUnknownServiceNameException() throw () {};

	const char* what() const throw () { return m_unknown_service_name.c_str(); };
	const std::string reason() const throw () { return m_unknown_service_name; };
private:
	std::string m_unknown_service_name;
};

class CAlreadyConnectedException: public std::exception {
public:
	CAlreadyConnectedException(std::string service_name, boost::asio::ip::tcp::endpoint endp) throw() : m_service_name(service_name), m_endp(endp) {};
 	virtual ~CAlreadyConnectedException() throw () {};

	const char* what() const throw () { return m_service_name.c_str(); };
	const std::string reason() const throw () { return m_service_name; };
private:
	const std::string m_service_name;
	const boost::asio::ip::tcp::endpoint m_endp;
};

class CConnectionFailedException: public std::exception {
public:
	CConnectionFailedException(std::string service_name, boost::asio::ip::tcp::endpoint endp) throw() : m_service_name(service_name), m_endp(endp) {};
 	virtual ~CConnectionFailedException() throw () {};

	const char* what() const throw () { return m_service_name.c_str(); };
	const std::string reason() const throw () { return m_service_name; };
private:
	const std::string m_service_name;
	const boost::asio::ip::tcp::endpoint m_endp;
};


#endif /* LIBMSTREAM_EXCEPTIONS_H_ */
