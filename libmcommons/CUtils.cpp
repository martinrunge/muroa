/***************************************************************************
 *
 *   CUtils.cpp
 *
 *   This file is part of libmuroa++                                       *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>              *
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
#include <errno.h>

#include <vector>

using namespace std;
using namespace boost::filesystem;

CUtils::CUtils() {
}

CUtils::~CUtils() {
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


long CUtils::str2long(std::string str) throw(std::invalid_argument) {
	errno = 0;
	char* endptr;
	long iVal = strtol( str.c_str(), &endptr, 10);
	if (errno != 0 || *endptr != '\0' ) {
		ostringstream oss;
		switch (errno) {
		case ERANGE:
			oss << "Could not convert string '" << str << "' to int (out of range).";
			break;
		case EINVAL:
			oss << "Could not convert string '" << str << "' to int.";
			break;
		default:
			oss << "Could not convert string '" << str << "' to int.";
			break;
		}
		throw invalid_argument(oss.str());
	}
	return iVal;
}

int32_t CUtils::str2int32(std::string str) throw(std::invalid_argument) {
	errno = 0;
	char* endptr;
	long iVal = strtol( str.c_str(), &endptr, 10);
	if (errno != 0 || *endptr != '\0' ) {
		switch (errno) {
		case ERANGE:
			throw invalid_argument("convert string to int (out of range)");
			break;
		case EINVAL:
			throw invalid_argument("convert string to int");
			break;
		default:
			throw invalid_argument("convert string to int");
			break;
		}
	}
//	if(iVal > UINT32_MAX) {
//		throw invalid_argument("convert string to int (out of range)");
//	}
//	if(iVal < UINT32_MIN) {
//		throw invalid_argument("convert string to int (out of range)");
//	}

	return iVal;
}


uint32_t CUtils::str2uint32(std::string str) throw(std::invalid_argument) {
	errno = 0;
	char* endptr;
	unsigned long iVal = strtoul( str.c_str(), &endptr, 10);
	if (errno != 0 || *endptr != '\0' ) {
		switch (errno) {
		case ERANGE:
			throw invalid_argument("convert string to int (out of range)");
			break;
		case EINVAL:
			throw invalid_argument("convert string to int");
			break;
		default:
			throw invalid_argument("convert string to int");
			break;
		}
	}
//	if(iVal > UINT32_MAX) {
//		throw invalid_argument("convert string to int (out of range)");
//	}
//	if(iVal < UINT32_MIN) {
//		throw invalid_argument("convert string to int (out of range)");
//	}

	return iVal;
}

int64_t CUtils::str2int64(std::string str) throw(std::invalid_argument) {
	errno = 0;
	char* endptr;
	long long iVal = strtoll( str.c_str(), &endptr, 10);
	if (errno != 0 || *endptr != '\0' ) {
		switch (errno) {
		case ERANGE:
			throw invalid_argument("convert string to int (out of range)");
			break;
		case EINVAL:
			throw invalid_argument("convert string to int");
			break;
		default:
			throw invalid_argument("convert string to int");
			break;
		}
	}
//	if(iVal > UINT32_MAX) {
//		throw invalid_argument("convert string to int (out of range)");
//	}
//	if(iVal < UINT32_MIN) {
//		throw invalid_argument("convert string to int (out of range)");
//	}

	return iVal;
}

uint64_t CUtils::str2uint64(std::string str) throw(std::invalid_argument) {
	errno = 0;
	char* endptr;
	unsigned long long iVal = strtoull( str.c_str(), &endptr, 10);
	if (errno != 0 || *endptr != '\0' ) {
		switch (errno) {
		case ERANGE:
			throw invalid_argument("convert string to int (out of range)");
			break;
		case EINVAL:
			throw invalid_argument("convert string to int");
			break;
		default:
			throw invalid_argument("convert string to int");
			break;
		}
	}
//	if(iVal > UINT32_MAX) {
//		throw invalid_argument("convert string to int (out of range)");
//	}
//	if(iVal < UINT32_MIN) {
//		throw invalid_argument("convert string to int (out of range)");
//	}

	return iVal;
}


void CUtils::xmlesc(std::string& input) {
    std::string buffer;
    buffer.reserve(input.size());
    for(size_t pos = 0; pos != input.size(); ++pos) {
        switch(input[pos]) {
            case '&':  buffer.append("&amp;");       break;
            case '\"': buffer.append("&quot;");      break;
            case '\'': buffer.append("&apos;");      break;
            case '<':  buffer.append("&lt;");        break;
            case '>':  buffer.append("&gt;");        break;
            default:   buffer.append(&input[pos], 1); break;
        }
    }
    input.swap(buffer);
}


path CUtils::expandvars(path p) {
	path rp;
	path::iterator it = p.begin();
	while (it != p.end()) {
		cerr << *it << endl;
		string elem(it->string());
		if(elem.compare("~") == 0) {
			elem = string(getenv("HOME"));
		}
		if(elem.find_first_of('$') == 0) {
			const char* envvar = getenv( elem.substr(1).c_str() );
			if(envvar != NULL) {
				elem = envvar;
			}
		}

		rp /= elem;
		cerr << "   " << rp << endl;
		++it;
	}
	return rp;
}
