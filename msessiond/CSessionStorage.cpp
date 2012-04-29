/*
 * CSessionStorage.cpp
 *
 *  Created on: 15 Apr 2012
 *      Author: martin
 */

#include "CSessionStorage.h"

#include <CApp.h>
#include <CRootItem.h>
#include "CSession.h"
#include <CUtils.h>

#include <limits.h>

using namespace std;
using namespace boost::filesystem;
using namespace muroa;

const string CSessionStorage::mediaColSubdir("mediacol");
const string CSessionStorage::playlistSubdir("playlist");
const string CSessionStorage::nextlistSubdir("nextlist");

const string CSessionStorage::mcrev_file_extension(".mcrev");

CSessionStorage::CSessionStorage(CSession* session)  : m_app(muroa::CApp::getInstPtr()),
                                                       m_session(session)
{
	m_storage_path = m_app->settings().getProptery("msessiond.sessions_storage_dir", "");
	m_storage_path/=m_session->getName();

	if(!exists(m_storage_path) || !is_directory(m_storage_path)) {
		create_directories(m_storage_path);
	}

	if(!exists(m_storage_path/mediaColSubdir)) {
		create_directory(m_storage_path/mediaColSubdir);
	}

	if(!exists(m_storage_path/playlistSubdir)) {
		create_directory(m_storage_path/playlistSubdir);
	}

	if(!exists(m_storage_path/nextlistSubdir)) {
		create_directory(m_storage_path/nextlistSubdir);
	}
}

CSessionStorage::~CSessionStorage()
{
}

void CSessionStorage::save() {
	CRootItem* ri;
	long minRev = m_session->getMinMediaColRev();
	long maxRev = m_session->getMaxMediaColRev();
	saveMediaColRevs(minRev, maxRev);

	minRev = m_session->getMinPlaylistRev();
	maxRev = m_session->getMaxPlaylistRev();
    savePlaylistRevs(minRev, maxRev);

	minRev = m_session->getMinNextlistRev();
	maxRev = m_session->getMaxNextlistRev();
	saveNextlistRevs(minRev, maxRev);
}

void CSessionStorage::saveMediaColRevs(long minRev, long maxRev) {
	assert(maxRev >= minRev);
	if(minRev == 0) minRev++;
	CRootItem* ri;
	for(long i = minRev; i <= maxRev; i++) {
		ri = m_session->getMediaCol(i);

		ostringstream oss;
		oss << i << ".mcrev";
		path fn = m_storage_path/mediaColSubdir/oss.str();
		if(!exists(fn)) {
			ri->serialize(fn.string());
		}
	}
}

void CSessionStorage::savePlaylistRevs(long minRev, long maxRev) {
	assert(maxRev >= minRev);
	if(minRev == 0) minRev++;
	CRootItem* ri;
	for(long i = minRev; i <= maxRev; i++) {
		ri = m_session->getPlaylist(i);

		ostringstream oss;
		oss << i << ".mcrev";
		path fn = m_storage_path/playlistSubdir/oss.str();
		if(!exists(fn)) {
			ri->serialize(fn.string());
		}
	}
}

void CSessionStorage::saveNextlistRevs(long minRev, long maxRev) {
	assert(maxRev >= minRev);
	if(minRev == 0) minRev++;
	CRootItem* ri;
	for(long i = minRev; i <= maxRev; i++) {
		ri = m_session->getNextlist(i);

		ostringstream oss;
		oss << i << ".mcrev";
		path fn = m_storage_path/nextlistSubdir/oss.str();
		if(!exists(fn)) {
			ri->serialize(fn.string());
		}
	}
}


void CSessionStorage::restore() {
	restoreMediaColRevs();
	restorePlaylistRevs();
	restoreNextlistRevs();
}

void CSessionStorage::cleanup() {

}

void CSessionStorage::restoreRootItemRevs( string subdir_name ) {

	unsigned long minRev = ULONG_MAX;
	unsigned long maxRev = 0;

	map<unsigned long, string> filenames;

	directory_iterator dir_it = directory_iterator( m_storage_path/subdir_name );
	while( dir_it != directory_iterator() )
	{
		string ext = dir_it->path().extension().string();
		if( ext.compare( mcrev_file_extension ) == 0 )
		{
			try {
				string revNrStr = dir_it->path().stem().string();
				unsigned long revNr = CUtils::str2uint32(revNrStr);
				maxRev = (revNr > maxRev) ? revNr : maxRev;
				minRev = (revNr < minRev) ? revNr : minRev;

				filenames.insert( pair<unsigned long, string>(revNr, dir_it->path().string()));
			}
			catch(invalid_argument& ex) {

			}
		}
		dir_it++;
	}

	// only use continous ranges. If there are gaps, we use revisions from the upper most gap
	// to maxRev.
	unsigned long usableMin = maxRev;

	// find upper most gap
	for(unsigned long i = maxRev; i >= minRev && filenames.find(i) != filenames.end(); i--) {
		usableMin = i;
	}

	if(subdir_name.compare(mediaColSubdir) == 0) {
		m_session->setMinMediaColRev( usableMin );
		for(unsigned long i = usableMin; i <= maxRev ; i++) {
			CRootItem*ri = new CRootItem();
			map<unsigned long, string>::iterator it = filenames.find(i);
			if(it != filenames.end()) {
				string fn = filenames.find(i)->second;
				ri->fromFile(fn);
				m_session->setMaxMediaColRev( ri->getRevision() - 1 );
				m_session->addMediaColRev(ri);
			}
		}
//		m_session->setMinMediaColRev( usableMin );
// 		m_session->setMaxMediaColRev( maxRev );  // max rev will be reached by calling addMediaVolRev() several times.

	}else if(subdir_name.compare(playlistSubdir) == 0) {
		m_session->setMinPlaylistRev( usableMin );
		for(unsigned long i = usableMin; i <= maxRev ; i++) {
			CRootItem*ri = new CRootItem();
			ri->fromFile(filenames.find(i)->second);
			m_session->setMaxPlaylistRev( ri->getRevision() - 1 );
			m_session->addPlaylistRev(ri);
		}
//		m_session->setMinPlaylistRev( usableMin );
//		m_session->setMaxPlaylistRev( maxRev );  // max rev will be reached by calling addMediaVolRev() several times.
	} else { // subdir_name.compare(nextlistSubdir) == 0
		m_session->setMinNextlistRev( usableMin );
		for(unsigned long i = usableMin; i <= maxRev ; i++) {
			CRootItem*ri = new CRootItem();
			ri->fromFile(filenames.find(i)->second);
			m_session->setMaxNextlistRev( ri->getRevision() - 1 );
			m_session->addNextlistRev(ri);
		}
//		m_session->setMinNextlistRev( usableMin );
//		m_session->setMaxNextlistRev( maxRev );  // max rev will be reached by calling addMediaVolRev() several times.
	}
}

void CSessionStorage::restoreMediaColRevs() {
	restoreRootItemRevs(mediaColSubdir);
}

void CSessionStorage::restorePlaylistRevs() {
	restoreRootItemRevs(playlistSubdir);
}

void CSessionStorage::restoreNextlistRevs() {
	restoreRootItemRevs(nextlistSubdir);
}
