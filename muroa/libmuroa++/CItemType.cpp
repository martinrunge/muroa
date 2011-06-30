/*
 * CItemType.cpp
 *
 *  Created on: 29 Jun 2011
 *      Author: martin
 */

#include "CItemType.h"

using namespace std;

std::map<unsigned, std::string> CItemType::m_type_str = { std::pair<int, std::string>(E_ROOT, "R"),
			                                              std::pair<int, std::string>(E_CAT, "C"),
			                                              std::pair<int, std::string>(E_MEDIAITEM, "M"),
                                                          std::pair<int, std::string>(E_PLAYLISTITEM, "P"),
			                                              std::pair<int, std::string>(E_NEXTLISTITEM, "N")};


CItemType::CItemType() {
	// TODO Auto-generated constructor stub

}

CItemType::~CItemType() {
	// TODO Auto-generated destructor stub
}

string CItemType::getString(item_type_t type) {
   	map<unsigned, string>::iterator it;
    it = m_type_str.find(type);
    if(it == m_type_str.end()) {
    	return "<unknown type>";
    }
    else {
    	return it->second;
    }
}
