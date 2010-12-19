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
#include <list>

#include <thread>

namespace fs = boost::filesystem;

class CMediaScanner;
class CMediaItem;

class CFsScanner {
	// for unittests
	friend class CMediaFileTest;

public:
	CFsScanner(CMediaScanner* parent );
	virtual ~CFsScanner();

	inline void setFileTypes(std::vector<std::string> types) { m_types = types; };
	inline std::vector<std::string> getFileTypes() { return m_types; };

	void scanDir(std::string dir);
	void scanDirBFS(std::string dir);
	std::vector<CMediaItem*>* finishScan();

private:
	int walkTree(std::string dir);
	int walkTreeBFS(std::string dir);

	void cleanupScanResult();

	bool knownType( fs::path path );
	CMediaItem* readTag( fs::path path );

	std::vector<std::string> m_types;

	std::vector<CMediaItem*>* m_scanResult;

	std::list<std::string> m_progressDirs;
	int m_progress_dirs_size;
	int m_progress;
	int m_progress_num_dirs;
	int m_progress_depth;

	struct iterstate {
		fs::path path;
		fs::directory_iterator dirIter;
		int depth;
	};

	bool m_scanning;
	std::thread m_thread;

	CMediaScanner* m_parent;
};

#endif /* CFSSCANNER_H_ */
