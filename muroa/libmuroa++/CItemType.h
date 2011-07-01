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
	enum item_type { E_INVAL,
		             E_ROOT,
		             E_CAT,
		             E_MEDIAITEM,
		             E_PLAYLISTITEM,
		             E_NEXTLISTITEM,
	                 E_USER_TYPE = 1000 };
	typedef enum item_type item_type_t;

	CItemType(const std::string typeStr);
	CItemType(const item_type_t type);
	virtual ~CItemType();


	inline const std::string getString() const { return getString(m_type); };
	inline const item_type_t getType() const { return m_type; };

	static std::string getString(const item_type_t type);
	static item_type_t getType(const std::string typeStr);

private:
	static std::map<unsigned, std::string> m_type2str;
	static std::map<std::string, unsigned> m_str2type;

	const item_type_t m_type;

	static std::map<std::string, unsigned> init_reverse_map();
};

typedef CItemType::item_type item_type_t;

#endif /* CITEMTYPE_H_ */
