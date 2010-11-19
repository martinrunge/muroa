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
#include <thread>

namespace fs = boost::filesystem;

class CMediaScanner;
class CMediaItem;

class CFsScanner {
public:
	CFsScanner(CMediaScanner* parent );
	virtual ~CFsScanner();

	inline void setFileTypes(std::vector<std::string> types) { m_types = types; };
	inline std::vector<std::string> getFileTypes() { return m_types; };

	void scanDir(std::string dir);
	std::vector<CMediaItem*>* finishScan();

private:
	int walkTree(std::string dir);

	bool knownType( fs::path path );
	CMediaItem* readTag( fs::path path );

	std::vector<std::string> m_types;

	std::vector<CMediaItem*>* m_scanResult;

	struct iterstate {
		fs::path path;
		fs::directory_iterator dirIter;
	};

	bool m_scanning;
	std::thread m_thread;

	CMediaScanner* m_parent;
};

#endif /* CFSSCANNER_H_ */
