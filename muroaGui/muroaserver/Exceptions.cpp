/*
 * Exceptions.cpp
 *
 *  Created on: 11 Sep 2010
 *      Author: martin
 */

#include "Exceptions.h"


CException::CException(std::string reason) throw() {
	m_reason = reason;
}

CException::~CException() throw() {
	// TODO Auto-generated destructor stub
}

const char* CException::what() const throw() {
	return m_reason.c_str();
}

const std::string CException::reason() const throw() {
	return m_reason;
}



CApiMisuseException::CApiMisuseException(std::string reason) throw() {
	m_reason = reason;
}

CApiMisuseException::~CApiMisuseException() throw() {
	// TODO Auto-generated destructor stub
}

const char* CApiMisuseException::what() const throw() {
	return m_reason.c_str();
}

const std::string CApiMisuseException::reason() const throw() {
	return m_reason;
}