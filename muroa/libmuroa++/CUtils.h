/***************************************************************************
 *
 *   CUtils.h
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

#ifndef CUTILS_H_
#define CUTILS_H_

#include <string>
#include <cstdint>
#include <stdexcept>

class CUtils {
public:
	CUtils();
	virtual ~CUtils();

	static std::string file2string(const std::string &fileName);

	static long str2long(std::string str) throw(std::invalid_argument);
	static uint32_t str2uint32(std::string str) throw(std::invalid_argument);

};

#endif /* CUTILS_H_ */
