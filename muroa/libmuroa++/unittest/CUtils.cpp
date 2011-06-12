/***************************************************************************
 *
 *   CUtils.cpp
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

#include "CUtils.h"
#include <sstream>
#include <fstream>
#include <iostream>

#include <vector>

using namespace std;

CUtils::CUtils() {
	// TODO Auto-generated constructor stub

}

CUtils::~CUtils() {
	// TODO Auto-generated destructor stub
}

string CUtils::file2string(const string &fileName)
{
    ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);

    if(ifs.is_open()) {
    	ifstream::pos_type fileSize = ifs.tellg();
    	ifs.seekg(0, ios::beg);

    	vector<char> bytes(fileSize);
    	ifs.read(&bytes[0], fileSize);

    	return string(&bytes[0], fileSize);
    }
    else {
    	cerr << "could not open file " << fileName << endl;
    	return string();
   	}
}
