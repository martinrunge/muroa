/*
 * CItemType.h
 *
 *  Created on: 29 Jun 2011
 *      Author: martin
 */

#ifndef CITEMTYPE_H_
#define CITEMTYPE_H_

#include <string>
#include <map>


class CItemType {
public:
	CItemType();
	virtual ~CItemType();

	enum item_type { E_ROOT,
		             E_CAT,
		             E_MEDIAITEM,
		             E_PLAYLISTITEM,
		             E_NEXTLISTITEM,
	                 E_USER_TYPE = 1000 };
	typedef enum item_type item_type_t;
	static std::string getString(item_type_t type);

private:
	static std::map<unsigned, std::string> m_type_str;
};

typedef CItemType::item_type item_type_t;

#endif /* CITEMTYPE_H_ */
