/*
 * CItemType.cpp
 *
 *  Created on: 29 Jun 2011
 *      Author: martin
 */

#include "CItemType.h"

using namespace std;

std::map<unsigned, std::string> CItemType::m_type2str = { std::pair<int, std::string>(E_INVAL, "<invalid type>"),
		                                                  std::pair<int, std::string>(E_ROOT, "R"),
			                                              std::pair<int, std::string>(E_CAT, "C"),
			                                              std::pair<int, std::string>(E_MEDIAITEM, "M"),
                                                          std::pair<int, std::string>(E_PLAYLISTITEM, "P"),
			                                              std::pair<int, std::string>(E_NEXTLISTITEM, "N")};

std::map<std::string, unsigned> CItemType::m_str2type = CItemType::init_reverse_map();


CItemType::CItemType(string typeStr) : m_type(getType(typeStr)) {}
CItemType::CItemType(item_type_t type) : m_type(type) {}
CItemType::~CItemType() {}

string CItemType::getString(item_type_t type) {
   	map<unsigned, string>::iterator it;
    it = m_type2str.find(type);
    if(it == m_type2str.end()) {
    	return "<invalid type>";
    }
    else {
    	return it->second;
    }
}

item_type_t CItemType::getType(string typeStr) {
   	map<string, unsigned>::iterator it;
    it = m_str2type.find(typeStr);
    if(it == m_str2type.end()) {
    	return E_INVAL;
    }
    else {
    	return static_cast<item_type_t>(it->second);
    }
}


map<string, unsigned>  CItemType::init_reverse_map() {
	map<string, unsigned> reverse_map;
	map<unsigned, string>::iterator it;
    for ( it = CItemType::m_type2str.begin() ; it != CItemType::m_type2str.end(); it++ ) {
    	reverse_map.insert(pair<string, unsigned>(it->second, it->first));
    }
    return reverse_map;
}
