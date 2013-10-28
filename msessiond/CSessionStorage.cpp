/*
 * CSessionStorage.cpp
 *
 *  Created on: 15 Apr 2012
 *      Author: martin
 */

#include "CSessionStorage.h"

#include <CApp.h>
#include <CRootItem.h>
#include <IContentItem.h>
#include <CStreamClientItem.h>
#include "CSession.h"
#include "mediaprocessing/CStream.h"
#include <CUtils.h>

#include <limits.h>

using namespace std;
using namespace boost::filesystem;
namespace muroa {

const string CSessionStorage::mediaColSubdir("mediacol");
const string CSessionStorage::playlistSubdir("playlist");
const string CSessionStorage::nextlistSubdir("nextlist");
const string CSessionStorage::sessionStateSubdir("session_state");

const string CSessionStorage::mcrev_file_extension(".mcrev");

CSessionStorage::CSessionStorage(CSession* session)  : m_app(muroa::CApp::getInstPtr()),
                                                       m_session(session)
{
	m_storage_path = m_app->settings().getProperty("msessiond.sessions_storage_dir", "");
	m_storage_path = CUtils::expandvars(m_storage_path);
	m_storage_path/=m_session->getName();

	LOG4CPLUS_INFO(m_app->logger(), "session storage in " << m_storage_path <<  endl );

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

	if(!exists(m_storage_path/sessionStateSubdir)) {
		create_directory(m_storage_path/sessionStateSubdir);
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

void CSessionStorage::saveSessionStateRevs(long minRev, long maxRev) {
	assert(maxRev >= minRev);
	if(minRev == 0) minRev++;
	CRootItem* ri;
	for(long i = minRev; i <= maxRev; i++) {
		ri = m_session->getSessionState(i);

		ostringstream oss;
		oss << i << ".mcrev";
		path fn = m_storage_path/sessionStateSubdir/oss.str();
		if(!exists(fn)) {
			ri->serialize(fn.string());
		}
	}
}


void CSessionStorage::restore(CStream* stream) {
	restoreMediaColRevs();
	restorePlaylistRevs();
	restoreNextlistRevs();
	restoreSessionStateRevs(stream);
}

void CSessionStorage::cleanup() {

}

path CSessionStorage::getMediaColPath() {
	return m_storage_path/mediaColSubdir;
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
				if(revNr != 0) { // 0 is empty dummy revision, do not load or save it!
					maxRev = (revNr > maxRev) ? revNr : maxRev;
					minRev = (revNr < minRev) ? revNr : minRev;
					filenames.insert( pair<unsigned long, string>(revNr, dir_it->path().string()));
				}
			}
			catch(invalid_argument& ex) {

			}
		}
		dir_it++;
	}

	// only use continuous ranges. If there are gaps, we use revisions from the upper most gap
	// to maxRev.
	unsigned long usableMin = maxRev;

	// find upper most gap
	for(unsigned long i = maxRev; i >= minRev && filenames.find(i) != filenames.end(); i--) {
		usableMin = i;
	}

	if(usableMin == 0 && maxRev == 0) {
		// if no saved revision were found, exit here. Do no try to load revision 0.
		return;
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
	} else if(subdir_name.compare(nextlistSubdir) == 0) {
		m_session->setMinNextlistRev( usableMin );
		for(unsigned long i = usableMin; i <= maxRev ; i++) {
			CRootItem*ri = new CRootItem();
			ri->fromFile(filenames.find(i)->second);
			m_session->setMaxNextlistRev( ri->getRevision() - 1 );
			m_session->addNextlistRev(ri);
		}
//		m_session->setMinNextlistRev( usableMin );
//		m_session->setMaxNextlistRev( maxRev );  // max rev will be reached by calling addMediaVolRev() several times.
	} else { // subdir_name.compare(sessionStateSubDir) == 0
		m_session->setMinSessionStateRev( usableMin );
		for(unsigned long i = usableMin; i <= maxRev ; i++) {
			CRootItem*ri = new CRootItem();
			ri->fromFile(filenames.find(i)->second);
			m_session->setMaxSessionStateRev( ri->getRevision() - 1 );
			m_session->addSessionStateRev(ri);
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

void CSessionStorage::restoreSessionStateRevs(CStream* stream) {
	restoreRootItemRevs(sessionStateSubdir);

	// check if all render clients are in the same state as in last revision
	int num_clients;
    CRootItem* sstate = m_session->getSessionState( );
    CRootItem* newSState = new CRootItem(*sstate);

    CCategoryItem* base = newSState->getCategoryPtr("/RenderClients");
    if(base == 0) {
        base = newSState->mkPath("/RenderClients");
        num_clients = 0;
    }
    else {
        num_clients = base->getNumContentItems();
    }

    for(int i=0; i < num_clients; i++) {
        IContentItem* ci = base->getContentItem(i);
        if(ci->type() == CItemType::E_STREAM_CLIENT) {
            CStreamClientItem* sci = reinterpret_cast<CStreamClientItem*>(ci);

            ServDescPtr srvPtr = m_session->getServiceByName(sci->getServiceName());
            string sessionName = m_session->getName();
            if(srvPtr) {
               // service is there, if it part of this session, enable it, else do nothing
                if(sessionName.compare(sci->getOwnerSessionName()) == 0) {
                    // client is not active, but this session has owned it -> keep it as disabled
                    sci->setEnabled(true);

                    // add this render client to streamserver
                    stream->addReceiver(srvPtr);
                }
            }
            else {
                // service is not there, if it is part of this session, disable it, else remove it
                if(sessionName.compare(sci->getOwnerSessionName()) == 0) {
                    // client is not active, but this session has owned it -> keep it as disabled
                    sci->setEnabled(false);
                }
                else {
                    base->delContentItem(i);
                    num_clients--;
                }
            }
        }
    }


    if( *newSState == *sstate ) {
        delete newSState;
    }
    else {
        m_session->addSessionStateRev(newSState);
    }


}
} // namespace muroa
