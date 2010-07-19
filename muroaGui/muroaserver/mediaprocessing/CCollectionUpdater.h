/*
 * CCollectionUpdater.h
 *
 *  Created on: 16 Jul 2010
 *      Author: martin
 */

#ifndef CCOLLECTIONUPDATER_H_
#define CCOLLECTIONUPDATER_H_

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#include <string>
#include <vector>

#include "../CCollection.h"

namespace fs = boost::filesystem;

class CColelctionItem;

class CCollectionUpdater {
public:
	CCollectionUpdater();
	virtual ~CCollectionUpdater();

	inline void setFileTypes(std::vector<std::string> types) { m_types = types; };
	inline std::vector<std::string> getFileTypes() { return m_types; };

	CCollection<CCollectionItem> walkTree(std::string dir);

private:
	bool knownType( fs::path path );
	CCollectionItem* readTag( fs::path path );

	std::vector<std::string> m_types;

	struct iterstate {
		fs::path path;
		fs::directory_iterator dirIter;
	};
};

#endif /* CCOLLECTIONUPDATER_H_ */
