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

class MalformedPatchEx: public std::exception {
public:
	MalformedPatchEx(std::string reason, int line_nr) {};
	virtual ~MalformedPatchEx() throw () {};

	std::string getReason() { return m_reason; };
	int getLineNr() { return m_linr_nr; };

	const char* what() { return m_reason.c_str(); }

private:
	int m_linr_nr;
	std::string m_reason;
};

class rpcError: public std::exception {
public:
	rpcError(std::string reason) {};
	virtual ~rpcError() throw () {};

	std::string getReason() { return m_reason; };

	const char* what() { return m_reason.c_str(); }

private:
	std::string m_reason;
};


#endif /* MUROAEXCEPTIONS_H_ */
