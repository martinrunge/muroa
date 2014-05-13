/**
 * \file  CSession.cpp
 * \date 31 Oct 2011
 * \author Martin Runge <martin dot runge at web dot de>
 */

#include "CSession.h"

#include <cmds/Cmd.h>
#include <cmds/CmdProgress.h>
#include <cmds/CmdFinished.h>
#include <cmds/SimpleCmds.h>
#include <cmds/CmdEditMediaCol.h>
#include <cmds/CmdEditNextlist.h>
#include <cmds/CmdEditSessionState.h>

#include <IContentItem.h>
#include <CNextlistItem.h>
#include <CPlaylistItem.h>

#include <CTcpServer.h>
#include "CUtils.h"
#include "CMediaScannerCtrl.h"
#include "CApp.h"
#include "CStateDB.h"
#include "CSessionContainer.h"
#include "CSessionStorage.h"
#include "avahi/CServiceDesc.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>

#include <boost/filesystem.hpp>

#include "CCmdDispatcher.h"

#include "../mmscanner/CMsgScanDir.h"
#include "../mmscanner/CMsgOpenDb.h"

#include <sstream>
#include <string>

#include <log4cplus/loggingmacros.h>


using namespace std;

namespace muroa {

/**
 * \class CSession
 * \brief This class holds the state of a session.
 *
 * All connections that belong to a session \b must use the same io_service!!!
 *
 */
CSession::CSession(string name,
		             boost::asio::io_service& io_service,
		             CSessionContainer* const sessionContainer)
							: m_io_service(io_service),
							  m_name(name),
							  m_streamClientHdl(this),
							  m_maxMediaColRev(0),
							  m_maxPlaylistRev(0),
							  m_maxNextlistRev(0),
							  m_maxSessionStateRev(0),
							  m_minMediaColRev(0),
							  m_minPlaylistRev(0),
							  m_minNextlistRev(0),
							  m_minSessionStateRev(0),
							  m_playlistPos(0),
							  m_sessionStorage(0),
							  m_stateDBFilename("state.db"),
							  m_stream(this),
							  m_plIdProvider(this),
							  m_app(CApp::getInstPtr()),
							  m_sessionContainer(sessionContainer)
{

	// all thee collection have an empty revision 0!
	m_mediaColRevs[m_maxMediaColRev] = new CRootItem();
	m_playlistRevs[m_maxPlaylistRev] = new CRootItem();
	m_nextlistRevs[m_maxNextlistRev] = new CRootItem();
	m_sessionStateRevs[m_maxSessionStateRev] = new CRootItem();

	m_cmdDispatcher = new CCmdDispatcher(this);

	m_mediaScanner = new CMediaScannerCtrl(this, io_service);

	m_sessionStorage = new CSessionStorage(this);
	m_sessionStorage->restore(&m_stream);
}


CSession::~CSession() {

	if(m_stateDB != 0) {
		m_stateDB->saveSession(this);
		m_stateDB->close();
		delete m_stateDB;
		m_stateDB = 0;
	}

	if(m_sessionStorage != 0) {
		delete m_sessionStorage;
		m_sessionStorage = 0;
	}

	for(int i = m_minNextlistRev; i <= m_maxNextlistRev; i++) {
		m_nextlistRevs.erase(i);
	}

	for(int i = m_minPlaylistRev; i <= m_maxPlaylistRev; i++) {
		m_playlistRevs.erase(i);
	}

	for(int i = m_minMediaColRev; i <= m_maxMediaColRev; i++) {
		m_mediaColRevs.erase(i);
	}

	for(int i = m_minSessionStateRev; i <= m_maxSessionStateRev; i++) {
		m_sessionStateRevs.erase(i);
	}

	delete m_cmdDispatcher;

}

void CSession::play()
{
	m_stream.play();
}

void CSession::pause()
{
	m_stream.pause();
}

void CSession::stop()
{
	m_stream.stop();
}

void CSession::prev()
{
	m_stream.stop();
	addNextlistRevFromPrevCmd();
	m_stream.play();
}

void CSession::next()
{
	m_stream.stop();
	addNextlistRevFromNextCmd();
	m_stream.play();
}

CMediaItem* CSession::getCurrentMediaItem() throw(ExInvMsg)
{
	CRootItem* curNextlist = getNextlist();
	IContentItem* citem1 = curNextlist->getBase()->getContentItem(0);  // first element always there
	if(!citem1) {
		throw ExInvMsg("Nextlist is empty. At least current playlist position must be in there. Playlist empty?");
	}
	else {
		assert(citem1->type() == CItemType::E_NEXTLISTITEM);
		CNextlistItem* nlItem = reinterpret_cast<CNextlistItem*>(citem1);
		// nlItem->getPlaylistItemHash() nlItem->getHash();

		CRootItem* curMediaCol = getMediaCol();
		IContentItem* citem2 = curMediaCol->getContentPtr(CItemType::E_MEDIAITEM, nlItem->getMediaItemHash());
		if(!citem2) {
			dumpLookupErrorToLog( "Error looking up MediaItem in media collection by hash provided by nextlistItem.", 0, nlItem );
			throw ExInvMsg("Error: Selected Song not found in media collection");
		}
		else {
			assert(citem2->type() == CItemType::E_MEDIAITEM);
			CMediaItem* mItem = reinterpret_cast<CMediaItem*>(citem2);
			return mItem;
		}
	}
	return 0;
}


void CSession::addConnection(CConnection* ptr) {
	ptr->setSession(this);
	m_connections.insert(ptr);
	m_connections_by_id.insert(std::pair<unsigned, CConnection*>(ptr->getID(), ptr));
	ptr->joinSession(m_name);
}

void CSession::removeConnection(CConnection* ptr) {
	m_connections.erase(ptr);
	// m_connections_by_id.
}

CRootItem*  CSession::getMediaCol(int revision) const throw(ExInvMsg) {
	unsigned rev = (revision < 0)?m_maxMediaColRev:revision;
	CRootItem* ri = getRev(m_mediaColRevs, rev, string("Media collection revision # not known in session "));
	return ri;
}

CRootItem*  CSession::getPlaylist(int revision) const throw(ExInvMsg) {
	unsigned rev = (revision < 0)?m_maxPlaylistRev:revision;
	CRootItem* ri = getRev(m_playlistRevs, rev, "Playlist revision # not known in session ");
	return ri;
}

CRootItem*  CSession::getNextlist(int revision) const throw(ExInvMsg) {
	unsigned rev = (revision < 0)?m_maxNextlistRev:revision;
	CRootItem* ri = getRev(m_nextlistRevs, rev, "Nextlist revision # not known in session ");
	return ri;
}

CRootItem*  CSession::getSessionState(int revision) const throw(ExInvMsg) {
	unsigned rev = (revision < 0)?m_maxSessionStateRev:revision;
	CRootItem* ri = getRev(m_sessionStateRevs, rev, "SessionState revision # not known in session ");
	return ri;
}

const string CSession::getMediaColDiff(unsigned fromRevision, int toRevision) const {
	unsigned toRev = (toRevision < 0)?m_maxMediaColRev:toRevision;

	CRootItem* fromItem = getRev(m_mediaColRevs, fromRevision, "Can't build media collection diff. Base revision # is unknown in session '");
	CRootItem* toItem = getRev(m_mediaColRevs, toRev, "Can't build media collection diff. Target revision # is unknown in session '");

	string diff = fromItem->diff(*toItem);
	return diff;
}

const string CSession::getPlaylistDiff(unsigned fromRevision, int toRevision) const {
	unsigned toRev = (toRevision < 0)?m_maxPlaylistRev:toRevision;

	CRootItem* fromItem = getRev(m_playlistRevs, fromRevision, "Can't build playlist diff. Base revision # is unknown in session '");
	CRootItem* toItem = getRev(m_playlistRevs, toRev, "Can't build playlist diff. Target revision # is unknown in session '");

	string diff = fromItem->diff(*toItem);
	return diff;
}

const string CSession::getNextlistDiff(unsigned fromRevision, int toRevision) const {
	unsigned toRev = (toRevision < 0)?m_maxNextlistRev:toRevision;

	CRootItem* fromItem = getRev(m_nextlistRevs, fromRevision, "Can't build nextlist diff. Base revision # is unknown in session '");
	CRootItem* toItem = getRev(m_nextlistRevs, toRev, "Can't build nextlist diff. Target revision # is unknown in session '");

	string diff = fromItem->diff(*toItem);
	return diff;
}

const string CSession::getSessionStateDiff(unsigned fromRevision, int toRevision) const {
	unsigned toRev = (toRevision < 0)?m_maxSessionStateRev:toRevision;

	CRootItem* fromItem = getRev(m_sessionStateRevs, fromRevision, "Can't build nextlist diff. Base revision # is unknown in session '");
	CRootItem* toItem = getRev(m_sessionStateRevs, toRev, "Can't build nextlist diff. Target revision # is unknown in session '");

	string diff = fromItem->diff(*toItem);
	return diff;
}

void CSession::addMediaColRev(CRootItem* ri) {
		m_maxMediaColRev++;
		ri->setRevision(m_maxMediaColRev);
		m_mediaColRevs.insert(pair<unsigned, CRootItem*>(m_maxMediaColRev, ri));
		m_sessionStorage->saveMediaColRevs(m_maxMediaColRev, m_maxMediaColRev);
}

void CSession::addMediaColRev(const string& mediaCol ) {
	CRootItem* ri = new CRootItem();
	ri->deserialize(mediaCol);
	addMediaColRev(ri);
}

void CSession::addPlaylistRev(CRootItem* ri) {
	m_maxPlaylistRev++;
	ri->setRevision(m_maxPlaylistRev);
	m_playlistRevs.insert(pair<unsigned, CRootItem*>(m_maxPlaylistRev, ri));
	m_sessionStorage->savePlaylistRevs(m_maxPlaylistRev, m_maxPlaylistRev);
}

void CSession::addPlaylistRev(const string& playlist) {
	CRootItem* ri = new CRootItem();
	ri->deserialize(playlist);
	addPlaylistRev(ri);
}

void CSession::addNextlistRev(CRootItem* ri) {
	m_maxNextlistRev++;
	ri->setRevision(m_maxNextlistRev);
	m_nextlistRevs.insert(pair<unsigned, CRootItem*>(m_maxNextlistRev, ri));
	m_sessionStorage->saveNextlistRevs(m_maxNextlistRev, m_maxNextlistRev);
}

void CSession::addNextlistRev(const string& nextlist) {
	CRootItem* ri = new CRootItem();
	ri->deserialize(nextlist);
	addNextlistRev(ri);
}

void CSession::addSessionStateRev(CRootItem* ri) {
	m_maxSessionStateRev++;
	ri->setRevision(m_maxSessionStateRev);
	m_sessionStateRevs.insert(pair<unsigned, CRootItem*>(m_maxSessionStateRev, ri));
	m_sessionStorage->saveSessionStateRevs(m_maxSessionStateRev, m_maxSessionStateRev);
}

void CSession::addSessionStateRev(const std::string& sessionState) {
	CRootItem* ri = new CRootItem();
	ri->deserialize(sessionState);
	addSessionStateRev(ri);
}


int CSession::addMediaColRevFromDiff(const string& mediaColDiff, unsigned diffFromRev) throw(ExInvMsg) {
	CRootItem* ri = 0;
	try {
		CRootItem* base = getRev(m_mediaColRevs, diffFromRev, "Can't apply media collection diff based on revision # because that revision is unknown in session '");

		ri = new CRootItem(*base);
		ri->patch(mediaColDiff);
		addMediaColRev(ri);
	}
	catch(ExMalformedPatch& ex) {
		if(ri != 0) delete ri;
	}
}

int CSession::addPlaylistRevFromDiff(const string& playlistDiff, unsigned diffFromRev) throw(ExInvMsg) {
	uint32_t oldrev = getMaxPlaylistRev();
	CRootItem* base = getRev(m_playlistRevs, diffFromRev, "Can't apply playlist diff based on revision # because that revision is unknown in session '");

	CRootItem* ri = new CRootItem(*base);
	ri->patch(playlistDiff);
	addPlaylistRev(ri);
//	uint32_t newrev = getMaxPlaylistRev();
//
//	if(newrev - oldrev == 1) {
//		m_stateDB->updatePlaylistRevsTable(this, oldrev, newrev);
//	}
}

int CSession::addNextlistRevFromDiff(const string& nextlistDiff, unsigned diffFromRev) throw(ExInvMsg) {
	uint32_t oldrev = getMaxNextlistRev();
	CRootItem* base = getRev(m_nextlistRevs, diffFromRev, "Can't apply nextlist diff based on revision # because that revision is unknown in session '");

	CRootItem* ri = new CRootItem(*base);
	ri->patch(nextlistDiff);
	addNextlistRev(ri);
//
//	uint32_t newrev = getMaxNextlistRev();
//	if(newrev - oldrev == 1) {
//		m_stateDB->updateNextlistRevsTable(this, oldrev, newrev);
//		m_sessionStorage->save();
//	}
}

int CSession::addSessionStateRevFromDiff(const std::string& sessionStateDiff, unsigned diffFromRev) throw(ExInvMsg) {
	uint32_t oldrev = getMaxSessionStateRev();
	CRootItem* base = getRev(m_sessionStateRevs, diffFromRev, "Can't apply diff to session state based on revision # because that revision is unknown in session '");

	CRootItem* ri = new CRootItem(*base);
	ri->patch(sessionStateDiff);
	addSessionStateRev(ri);
}


int CSession::addNextlistRevFromNextCmd() {
	CRootItem *oldNextlist = getNextlist();
	CRootItem* newNextlist = new CRootItem( getMaxNextlistRev() + 1 );

	CCategoryItem* onlBase = oldNextlist->getBase();
	int numEntries = onlBase->getNumContentItems();

	ostringstream oss;

	if(numEntries <= 1)   // next would make nextlist be empty -> get next item from playlist
	{
		IContentItem* ci = onlBase->getContentItem(0);
		assert(ci->type() == CItemType::E_NEXTLISTITEM );
		CNextlistItem* curNlItem = reinterpret_cast<CNextlistItem*>(ci);

		CRootItem* playlist = getPlaylist();
		CCategoryItem* plBase = playlist->getBase();
		ci = playlist->getContentPtr(CItemType(CItemType::E_PLAYLISTITEM), curNlItem->getPlaylistItemHash() );
		ci = plBase->getSuccessorOf(ci);
		m_playlistPos++;
		if(ci == 0) {
			ci = plBase->getContentItem(0);
			m_playlistPos = 0;
		}
		assert(ci->type() == CItemType::E_PLAYLISTITEM);
		CPlaylistItem* nextPlItem = reinterpret_cast<CPlaylistItem*>(ci);

		CNextlistItem* newNlItem = new CNextlistItem(newNextlist, newNextlist->getBase());
		newNlItem->setMediaItemHash(nextPlItem->getMediaItemHash());
		newNlItem->setPlaylistItem(nextPlItem);
		newNextlist->addContentItem(newNlItem, newNextlist->getBase());

		oss << "@@ -1,1 +1,1 @@" << endl;
		oss << "-" << newNextlist->getBase()->getPath() << "\t" << onlBase->getContentItem(0)->serialize();
		oss << "+" << newNextlist->getBase()->getPath() << "\t" << newNlItem->serialize();
	}
	else
	{
		for(int i = 1; i < onlBase->getNumContentItems(); i++) {
			IContentItem* ci = onlBase->getContentItem(i);
			assert(ci->type() == CItemType::E_NEXTLISTITEM );
			newNextlist->addContentItem(ci, newNextlist->getBase());
		}
		IContentItem* it = newNextlist->getBase()->getContentItem(0);
		assert(it->type() == CItemType::E_NEXTLISTITEM);
		CNextlistItem* nlit0 = reinterpret_cast<CNextlistItem*>(it);
		m_playlistPos = nlit0->getPlaylistItemHash();

		oss << "@@ -1,1 +0,0 @@" << endl;
		oss << "-" << newNextlist->getBase()->getPath() << "\t" << onlBase->getContentItem(0)->serialize();
	}
	int oldNlRev = getMaxNextlistRev();

	addNextlistRev(newNextlist);

	CmdEditNextlist *cmdEdNl = new CmdEditNextlist(oldNlRev, getMaxNextlistRev(), oss.str() );
	toAll(cmdEdNl);
}

int CSession::addNextlistRevFromPrevCmd() {

}


CRootItem*  CSession::getRev(const map<unsigned, CRootItem*>& collection,
		                     const unsigned rev,
		                     const string& message) const throw(ExInvRev)
{
	map<unsigned, CRootItem*>::const_iterator it;
	it = collection.find(rev);
	if(it == collection.end()) {
		// revision not found
		int hashpos = message.find_first_of("#", 1);
		if( hashpos != string::npos ) {
			ostringstream oss;
			oss << message.substr(0, hashpos) << rev << message.substr( hashpos + 1, message.size() - hashpos - 1 ) << m_name << "'.";
			throw ExInvRev(oss.str(), rev);
		}
		else {
			throw ExInvRev(message + " '" + m_name + "'.", rev);
		}
	}
	return it->second;
}

void CSession::scanCollection(CConnection* initiator, uint32_t jobID) {
	// bookkeeping: remember, which connection ordered to scan the collection
	m_job_initiators.insert(std::pair<uint32_t, CConnection*>(jobID, initiator));

	m_mediaScanner->start();
	boost::filesystem::path dbpath = CApp::settings().getProperty("msessiond.sessions_storage_dir", "./") + getName() + "/mediacol";
	dbpath = CUtils::expandvars(dbpath);
	CMsgOpenDb* dbmsg = new CMsgOpenDb( dbpath.string() );
	m_mediaScanner->sendMsg(dbmsg);
	addOutstandingMsg(dbmsg);

	boost::filesystem::path mediapath = CApp::settings().getProperty("msessiond.media_path", "~/Desktop");
	mediapath = CUtils::expandvars(mediapath);
	CMsgScanDir* sdmsg = new CMsgScanDir( mediapath.string() );
	m_mediaScanner->sendMsg(sdmsg);
	addOutstandingMsg(sdmsg);

	LOG4CPLUS_INFO(m_app->logger(), "scan collegion: searching for media files in '" << mediapath << "'" << endl );

	setClientCmdIdBySubprocessCmdID(sdmsg->getID(), initiator, jobID );
}

void CSession::scanProgress(uint32_t jobID, uint32_t progress) {
	client_job_t cljob;
	try {
		cljob = getClientCmdIdBySubprocessCmdID(jobID, false);

		CmdProgress* progCmd = new CmdProgress(cljob.second, progress);
		cljob.first->sendCmd(progCmd);
	}catch(ExInvMsg& ex) {

	}
}

void CSession::jobFinished(uint32_t jobID) {
	client_job_t clientJob;
	try {
		clientJob = getClientCmdIdBySubprocessCmdID(jobID);

		CmdFinished* finiCmd = new CmdFinished(clientJob.second);
		clientJob.first->sendCmd(finiCmd);

	}catch(ExInvMsg& ex) {
		;
	}

	delOutstandingMsg(jobID);
}

void CSession::collectionChanged(uint32_t newRev, uint32_t minRev, uint32_t maxRev) {
	ostringstream oss;

	oss << m_sessionStorage->getMediaColPath().string() << "/" << newRev << m_sessionStorage->getMcrevFileExtension();
	// CRootItem* newMediaColRev = m_stateDB->getMediaColRev(newRev);
	CRootItem* newMediaColRev = new CRootItem();
	newMediaColRev->fromFile(oss.str());

	unsigned currentRev = m_maxMediaColRev;
	CRootItem* currentMediaCol = m_mediaColRevs[currentRev];

	addMediaColRev(newMediaColRev);

	string diff_or_serialisation;
	if( currentRev == 0 ) {
		diff_or_serialisation = newMediaColRev->serialize();
	}
	else {
		diff_or_serialisation = currentMediaCol->diff(*newMediaColRev);
	}

	CmdEditMediaCol* cmd = new CmdEditMediaCol(currentRev, m_maxMediaColRev, diff_or_serialisation);
	toAll(cmd);

	assert(newRev == m_maxMediaColRev);
}

void CSession::response(uint32_t requestID, int32_t returnCode, string message) {
	delOutstandingMsg(requestID);

}

void CSession::reportError(uint32_t jobID, int32_t errCode, string message) {
	delOutstandingMsg(jobID);

}

void CSession::incomingCmd(Cmd*  cmd, CConnection* initiator) {
	if(initiator != 0) {
		assert(m_job_initiators.find(cmd->id()) == m_job_initiators.end());  // this job ID should be new
		m_job_initiators[cmd->id()] = initiator;
	}
	m_cmdDispatcher->incomingCmd(cmd);
}

void CSession::postIncomingCmd(Cmd* cmd) {
	m_cmd_queue.push(cmd);
	m_io_service.post( boost::bind(&CSession::dequeueCmd, this) );
}

void CSession::dequeueCmd() {
	Cmd* cmd = m_cmd_queue.front();
	m_cmd_queue.pop();
	incomingCmd(cmd, 0);
}

void CSession::sendToInitiator(Cmd* cmd, unsigned connId) {
	map<unsigned, CConnection*>::iterator it = m_connections_by_id.find(connId);
	if ( it != m_connections_by_id.end() ) {
		CConnection* ini_conn = it->second;
		ini_conn->sendCmd(cmd);
	}
}

void CSession::toAll( Cmd* cmd, bool deleteCmd ) {
	set<CConnection*>::iterator it = m_connections.begin();
	while( it != m_connections.end() ) {
		(*it)->sendCmd(cmd);
		++it;
	}
	if(deleteCmd) {
		delete cmd;
	}
}

string CSession::getProperty(string key, string defaultVal) {
	string privKey = privatePropertyKey(key);
	string value = m_app->settings().getProperty(privKey, defaultVal);
	return value;
}

void CSession::setProperty(string key, string val) {
	string privKey = privatePropertyKey(key);
	m_app->settings().setProperty(privKey, val);
}

int CSession::getProperty(string key, int defaultVal) {
	string privKey = privatePropertyKey(key);
	int value = m_app->settings().getProptery(privKey, defaultVal);
	return value;
}

void CSession::setProperty(string key, int val) {
	string privKey = privatePropertyKey(key);
	m_app->settings().setProptery(privKey, val);
}

void CSession::addClient(std::string name) {
	pair<CRootItem*, string> result;
	result = m_streamClientHdl.addClientStateDiff(getSessionState(), name);
	if(result.first == 0) {
	    return;
	}
	int curSessionStateRev = getMaxSessionStateRev();

	addSessionStateRev( result.first );
	string stateDiff;
	if(curSessionStateRev == 0) {
	    stateDiff = result.first->serialize();
	}
	else {
	    stateDiff = result.second;
	}

	CmdEditSessionState* cmd_edss = new CmdEditSessionState(curSessionStateRev, getMaxSessionStateRev(), stateDiff );
	toAll(cmd_edss);

	m_stream.addReceiver( getServiceByName(name) );
}

void CSession::rmClient(std::string name) {
	pair<CRootItem*, string> result;
	result = m_streamClientHdl.rmClientStateDiff(getSessionState(), name);
    if(result.first != 0) {
        int curSessionStateRev = getMaxSessionStateRev();
        addSessionStateRev( result.first );
        string stateDiff = result.second;

        CmdEditSessionState* cmd_edss = new CmdEditSessionState(curSessionStateRev, getMaxSessionStateRev(), stateDiff );
        toAll(cmd_edss);

        m_stream.rmReceiver( name );
    }
}

void CSession::takeClient(std::string name, std::string ownerSessionsName) {
	pair<CRootItem*, string> result;
	result = m_streamClientHdl.takeClientStateDiff(getSessionState(), name, ownerSessionsName);
	int curSessionStateRev = getMaxSessionStateRev();

	addSessionStateRev( result.first );
	string stateDiff = result.second;

	CmdEditSessionState* cmd_edss = new CmdEditSessionState(curSessionStateRev, getMaxSessionStateRev(), stateDiff );
	toAll(cmd_edss);
}


ServDescPtr CSession::getServiceByName(std::string name) {
	return m_sessionContainer->getServiceByName(name);
}


/*
set<string>& CSession::getClients() const {
	return m_playback_clients;
}

void CSession::setClients(set<string> clients) {
	m_playback_clients = clients;
}
*/





bool CSession::operator==(const CSession& other) {

	if(m_name.compare(other.m_name) != 0) return false;

	if(m_minNextlistRev != other.m_minNextlistRev ) return false;
	if(m_maxNextlistRev != other.m_maxNextlistRev ) return false;
	if(m_minPlaylistRev != other.m_minPlaylistRev ) return false;
	if(m_maxPlaylistRev != other.m_maxPlaylistRev ) return false;
	if(m_minMediaColRev != other.m_minMediaColRev ) return false;
	if(m_maxMediaColRev != other.m_maxMediaColRev ) return false;

	for(int i = m_minNextlistRev; i <= m_maxNextlistRev; i++) {
		CRootItem* nl = m_nextlistRevs.find(i)->second;
		CRootItem* onl = other.m_nextlistRevs.find(i)->second;
		if( *nl != *onl ) return false;
	}

	for(int i = m_minPlaylistRev; i <= m_maxPlaylistRev; i++) {
		CRootItem* pl = m_playlistRevs.find(i)->second;
		CRootItem* opl = other.m_playlistRevs.find(i)->second;
		if( *pl != *opl ) return false;
	}

	for(int i = m_minMediaColRev; i <= m_maxMediaColRev; i++) {
		CRootItem* mc = m_mediaColRevs.find(i)->second;
		CRootItem* omc = other.m_mediaColRevs.find(i)->second;
		if( *mc != *omc ) return false;
	}

	return true;
}


string CSession::privatePropertyKey(string key) {
	string privKey = "session." + m_name + "." + key;
	return privKey;
}

void CSession::addOutstandingMsg(CMsgBase* msg) {
	pair<uint32_t, CMsgBase*> p(msg->getID(), msg);
	m_outstanding_msgs.insert(p);
}

void CSession::delOutstandingMsg(uint32_t id) throw(ExInvMsg) {
	map<uint32_t, CMsgBase*>::iterator it = m_outstanding_msgs.find(id);
	if(it == m_outstanding_msgs.end() ) {
		ostringstream oss;
		oss << "no command with id '" << id << "' to childprocess outstanding.";
		throw ExInvMsg(oss.str());
	}
	CMsgBase* msg = it->second;

	// cancel timer here

	m_outstanding_msgs.erase(it);
}

muroa::CSession::client_job_t CSession::getClientCmdIdBySubprocessCmdID(uint32_t subprocess_cmd_id, bool delentry) throw(ExInvMsg) {
	map<uint32_t, client_job_t>::iterator it = m_subprocess_job_by_cmdID.find(subprocess_cmd_id);
	if(it == m_subprocess_job_by_cmdID.end()) {
		ostringstream oss;
		oss << "no client is waiting for an answer to subprocess command '" << subprocess_cmd_id << "'";
		throw ExInvMsg( oss.str() );
	}
	client_job_t client = it->second;

	if(delentry) {
		m_subprocess_job_by_cmdID.erase(it);
	}

	return client;
}

void CSession::setClientCmdIdBySubprocessCmdID(uint32_t subprocess_cmd_id, CConnection* initiator, uint32_t client_cmd_id ) {
	client_job_t client(initiator, client_cmd_id);
	pair<uint32_t, client_job_t > entry(subprocess_cmd_id, client);
	m_subprocess_job_by_cmdID.insert(entry);
}

void CSession::dumpLookupErrorToLog(std::string descr, CPlaylistItem* plItem, CNextlistItem* nlItem) {
	ostringstream oss;

	oss << "CSession::dumpLookupErrorToLog: " << descr << endl;
	if(nlItem) {
		oss << "NextlistItem: " << nlItem->serialize() << endl;
	}
	if(plItem) {
		oss << "PlaylistItem: " << plItem->serialize() << endl;
	}
	oss << "MediaCol: " << getMediaCol()->serialize() << endl;
	LOG4CPLUS_ERROR(CApp::getInstPtr()->logger(), oss.str());
}

} /* namespace muroa */
