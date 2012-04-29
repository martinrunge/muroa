/*
 * CSessionStorage.h
 *
 *  Created on: 15 Apr 2012
 *      Author: martin
 */

#ifndef CSESSIONSTORAGE_H_
#define CSESSIONSTORAGE_H_

#include <string>
#include <boost/filesystem.hpp>

namespace muroa {
	class CApp;
	class CSession;
}

class CRootItem;

class CSessionStorage {
public:
	CSessionStorage(muroa::CSession* session);
	virtual ~CSessionStorage();

	void save();
	void saveMediaColRevs(long minRev, long maxRev);
	void savePlaylistRevs(long minRev, long maxRev);
	void saveNextlistRevs(long minRev, long maxRev);

	void restore();

	void cleanup();

private:
	muroa::CApp* m_app;
	muroa::CSession* m_session;

	boost::filesystem::path m_storage_path;

	void restoreMediaColRevs();
	void restorePlaylistRevs();
	void restoreNextlistRevs();

	void restoreRootItemRevs( std::string subdir_name );

	static const std::string mediaColSubdir;
	static const std::string playlistSubdir;
	static const std::string nextlistSubdir;

	static const std::string mcrev_file_extension;
};

#endif /* CSESSIONSTORAGE_H_ */
