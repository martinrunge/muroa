/*
 * CFsScanner.h
 *
 *  Created on: 16 Jul 2010
 *      Author: martin
 */

#ifndef CFSSCANNER_H_
#define CFSSCANNER_H_

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#include <string>
#include <vector>

namespace fs = boost::filesystem;

class CMediaItem;

class CFsScanner {
public:
	CFsScanner();
	virtual ~CFsScanner();

	inline void setFileTypes(std::vector<std::string> types) { m_types = types; };
	inline std::vector<std::string> getFileTypes() { return m_types; };

	std::vector<CMediaItem*>* walkTree(std::string dir);

private:
	bool knownType( fs::path path );
	CMediaItem* readTag( fs::path path );

	std::vector<std::string> m_types;

	struct iterstate {
		fs::path path;
		fs::directory_iterator dirIter;
	};
};

#endif /* CFSSCANNER_H_ */
