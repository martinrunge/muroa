/***************************************************************************
 *
 *   MuroaExceptions.cpp
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

#include "MuroaExceptions.h"

MalformedPatchEx::MalformedPatchEx(std::string reason, int line_nr) : m_reason(reason), m_linr_nr(line_nr)
{

}

MalformedPatchEx::~MalformedPatchEx() throw ()
{

}

std::string MalformedPatchEx::getReason()
{
	return m_reason;
}

int MalformedPatchEx::getLineNr()
{
	return m_linr_nr;
}

const char* MalformedPatchEx::what() const throw()
{
	return m_reason.c_str();
}


rpcError::rpcError(std::string reason) : m_reason(reason)
{

};

rpcError::~rpcError() throw ()
{

};

std::string rpcError::getReason()
{
	return m_reason;
}

const char* rpcError::what() const throw()
{
	return m_reason.c_str();
}

