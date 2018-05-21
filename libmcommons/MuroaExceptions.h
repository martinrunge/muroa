/***************************************************************************
 *
 *   MuroaExceptions.h
 *
 *   This file is part of libmuroa++                                  *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef MUROAEXCEPTIONS_H_
#define MUROAEXCEPTIONS_H_

#include <exception>
#include <string>
#include <sstream>

// namespace muroa {  // activate later on

/*! \class ExInvMsg
 *  \extends std::exception
 *  \brief general error in a received message and base for more detailed exceptions
 *
 *  Intended as a base for more detailed exceptions. It spans
 *  all errors that can happen during message processing in client and server
 */
class ExInvMsg: public std::exception {
public:
	ExInvMsg(std::string reason) throw (): m_reason(reason)
    {};
	virtual ~ExInvMsg() throw ()
    {};

    inline const char* what() const throw ()
    {
    	return m_reason.c_str();
    };

    inline const std::string reason() const throw ()
    {
    	return m_reason;
    };

private:
	std::string m_reason;
};

/*! \class ExMalformedPatch
 *  \extends ExInvMsg
 *  \brief Some error within a patch
 *
 *  The patch is somehow malformed and could not be parsed or
 *  the patch could not be applied for some other reasons, e.g.
 *  mismatching base. The case of a mismatching base should be
 *  caught before by an \see ExInvRev during normal operations.
 */
class ExMalformedPatch: public ExInvMsg {
public:
	ExMalformedPatch(std::string reason, int line_nr): ExInvMsg(reason), m_linr_nr(line_nr)
	{
	};
	virtual ~ExMalformedPatch() throw ()
	{
	};

	inline int getLineNr()
	{
		return m_linr_nr;
	};

private:
	int m_linr_nr;
};

/*! \class ExInvRev
 *  \extends ExInvMsg
 *  \brief Exception to report invalid revision number
 *
 *  can be unknown or just wrong revision number and also
 *  revision numbers that have been valid in the past, but
 *  have been cleaned up in between, so they are not known
 *  any more.
 */
class ExInvRev: public ExInvMsg {
public:
	ExInvRev(std::string reason, int revNr) : ExInvMsg(reason), m_rev_nr(revNr)
	{
	};
	virtual ~ExInvRev() throw ()
	{
	};

	inline int getLineNr()
	{
		return m_rev_nr;
	};

private:
	int m_rev_nr;
};



class ExRpcError: public std::exception {
public:
	ExRpcError(std::string reason) : m_reason(reason)
	{
	};
	virtual ~ExRpcError() throw ()
	{
	};

	inline const std::string getReason() const throw()
	{
		return m_reason;
	};

	inline const char* what() const throw()
	{
		return m_reason.c_str();
	};

private:
	std::string m_reason;
};

class ExRessourceNotFound: public std::exception {
public:
	ExRessourceNotFound(std::string& reason) : m_reason(reason) {
	};
	virtual ~ExRessourceNotFound()
	{
	};

	inline const std::string& getReason() const throw()
	{
		return m_reason;
	};

	inline const char* what() const throw()
	{
		return m_reason.c_str();
	};

private:
	const std::string m_reason;
};

namespace muroa {
class InterruptedEx: public std::exception {
};

} // namespace muroa

#endif /* MUROAEXCEPTIONS_H_ */
