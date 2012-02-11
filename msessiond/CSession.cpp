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

#include <CTcpServer.h>
#include "CMediaScannerCtrl.h"
#include "CApp.h"
#include "CStateDB.h"

#include "../mmscanner/CMsgScanDir.h"
#include "../mmscanner/CMsgOpenDb.h"

#include <sstream>


using namespace std;

namespace muroa {

/**
 * \class CSession
 * \brief This class holds the state of a session.
 *
 * All connections that belong to a session \b must use the same io_service!!!
 *
 */
CSession::CSession(string name, boost::asio::io_service& io_service) : m_name(name),
                                                                      m_maxMediaColRev(0),
                                                                      m_maxPlaylistRev(0),
                                                                      m_maxNextlistRev(0),
                                                                      m_minMediaColRev(0),
                                                                      m_minPlaylistRev(0),
                                                                      m_minNextlistRev(0),
                                                                      m_playlistPos(0),
                                                                      m_stateDBFilename("state.db"),
                                                                      m_app(CApp::getInstPtr()){

	// all thee collection have an empty revision 0!
	m_mediaColRevs[m_maxMediaColRev] = new CRootItem();
	m_playlistRevs[m_maxPlaylistRev] = new CRootItem();
	m_nextlistRevs[m_maxNextlistRev] = new CRootItem();

	m_mediaScanner = new CMediaScannerCtrl(this, io_service);

	m_stateDB = new CStateDB(m_stateDBFilename);
	m_stateDB->open();
	m_stateDB->restoreSession(this);
}


// this c-tor is intended for unittest only
CSession::CSession( std::string name ) :    m_name(name),
											m_maxMediaColRev(0),
											m_maxPlaylistRev(0),
											m_maxNextlistRev(0),
											m_minMediaColRev(0),
											m_minPlaylistRev(0),
											m_minNextlistRev(0),
											m_playlistPos(0),
											m_stateDB(0),
											m_stateDBFilename("unittest_state.db"),
											m_app(CApp::getInstPtr()) {

	// all thee collection have an empty revision 0!
	m_mediaColRevs[m_maxMediaColRev] = new CRootItem();
	m_playlistRevs[m_maxPlaylistRev] = new CRootItem();
	m_nextlistRevs[m_maxNextlistRev] = new CRootItem();

}

CSession::~CSession() {

	if(m_stateDB != 0) {
		m_stateDB->saveSession(this);
		m_stateDB->close();
		delete m_stateDB;
		m_stateDB = 0;
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

}

void CSession::addConnection(CConnection* ptr) {
	m_connections.insert(ptr);
	ptr->joinSession(m_name);
}

void CSession::removeConnection(CConnection* ptr) {
	m_connections.erase(ptr);
}

CRootItem*  CSession::getMediaCol(int revision) const {
	unsigned rev = (revision < 0)?m_maxMediaColRev:revision;
	CRootItem* ri = getRev(m_mediaColRevs, rev, string("Media collection revision # not known in session "));
	return ri;
}

CRootItem*  CSession::getPlaylist(int revision) const {
	unsigned rev = (revision < 0)?m_maxPlaylistRev:revision;
	CRootItem* ri = getRev(m_playlistRevs, rev, "Playlist revision # not known in session ");
	return ri;
}

CRootItem*  CSession::getNextlist(int revision) const {
	unsigned rev = (revision < 0)?m_maxNextlistRev:revision;
	CRootItem* ri = getRev(m_nextlistRevs, rev, "Nextlist revision # not known in session ");
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

	CRootItem* fromItem = getRev(m_playlistRevs, fromRevision, "Can't build nextlist diff. Base revision # is unknown in session '");
	CRootItem* toItem = getRev(m_playlistRevs, toRev, "Can't build nextlist diff. Target revision # is unknown in session '");

	string diff = fromItem->diff(*toItem);
	return diff;
}


void CSession::addMediaColRev(CRootItem* ri) {
	m_maxMediaColRev++;
	m_mediaColRevs.insert(pair<unsigned, CRootItem*>(m_maxMediaColRev, ri));
}

void CSession::addMediaColRev(const string& mediaCol ) {
	m_maxMediaColRev++;
	CRootItem* ri = new CRootItem();
	ri->deserialize(mediaCol);
	m_mediaColRevs.insert(pair<unsigned, CRootItem*>(m_maxMediaColRev, ri));
}

void CSession::addPlaylistRev(CRootItem* ri) {
	m_maxPlaylistRev++;
	m_playlistRevs.insert(pair<unsigned, CRootItem*>(m_maxPlaylistRev, ri));
}

void CSession::addPlaylistRev(const string& playlist) {
	m_maxPlaylistRev++;
	CRootItem* ri = new CRootItem();
	ri->deserialize(playlist);
	m_playlistRevs.insert(pair<unsigned, CRootItem*>(m_maxPlaylistRev, ri));
}

void CSession::addNextlistRev(CRootItem* ri) {
	m_maxNextlistRev++;
	m_nextlistRevs.insert(pair<unsigned, CRootItem*>(m_maxNextlistRev, ri));
}

void CSession::addNextlistRev(const string& nextlist) {
	m_maxNextlistRev++;
	CRootItem* ri = new CRootItem();
	ri->deserialize(nextlist);
	m_nextlistRevs.insert(pair<unsigned, CRootItem*>(m_maxNextlistRev, ri));
}

int CSession::addMediaColRevFromDiff(const string& mediaColDiff, unsigned diffFromRev) throw(InvalidMsgException) {
	CRootItem* base = getRev(m_mediaColRevs, diffFromRev, "Can't apply media collection diff based on revision # because that revision is unknown in session '");

	m_maxMediaColRev++;
	CRootItem* ri = new CRootItem(*base);
	ri->patch(mediaColDiff);
	m_mediaColRevs.insert(pair<unsigned, CRootItem*>(m_maxMediaColRev, ri));
}

int CSession::addPlaylistRevFromDiff(const string& playlistDiff, unsigned diffFromRev) throw(InvalidMsgException) {
	CRootItem* base = getRev(m_playlistRevs, diffFromRev, "Can't apply playlist diff based on revision # because that revision is unknown in session '");

	m_maxPlaylistRev++;
	CRootItem* ri = new CRootItem(*base);
	ri->patch(playlistDiff);
	m_playlistRevs.insert(pair<unsigned, CRootItem*>(m_maxPlaylistRev, ri));
}

int CSession::addNextlistRevFromDiff(const string& nextlistDiff, unsigned diffFromRev) throw(InvalidMsgException) {
	CRootItem* base = getRev(m_nextlistRevs, diffFromRev, "Can't apply nextlist diff based on revision # because that revision is unknown in session '");

	m_maxNextlistRev++;
	CRootItem* ri = new CRootItem(*base);
	ri->patch(nextlistDiff);
	m_nextlistRevs.insert(pair<unsigned, CRootItem*>(m_maxNextlistRev, ri));
}

int CSession::addNextlistRevFromNextCmd() {

}

int CSession::addNextlistRevFromPrevCmd() {

}

void CSession::setMinMediaColRevision(int rev) throw() {
	m_minMediaColRev = rev;
}

void CSession::setMinPlaylistRevision(int rev) throw() {
	m_minPlaylistRev = rev;
}

void CSession::setMinNextlistRevision(int rev) throw() {
	m_minNextlistRev = rev;
}



CRootItem*  CSession::getRev(const map<unsigned, CRootItem*>& collection,
		                     const unsigned rev,
		                     const string& message) const throw(InvalidMsgException)
{
	map<unsigned, CRootItem*>::const_iterator it;
	it = collection.find(rev);
	if(it == collection.end()) {
		// revision not found
		int hashpos = message.find_first_of("#", 1);
		if( hashpos != string::npos ) {
			ostringstream oss;
			oss << message.substr(0, hashpos) << rev << message.substr( hashpos + 1, message.size() - hashpos - 1 ) << m_name << "'.";
			throw InvalidMsgException(oss.str());
		}
		else {
			throw InvalidMsgException(message + " '" + m_name + "'.");
		}
	}
	return it->second;
}

void CSession::scanCollection(CConnection* initiator, uint32_t jobID) {
	// bookkeeping: remember, which connection ordered to scan the collection
	m_job_initiators.insert(std::pair<uint32_t, CConnection*>(jobID, initiator));

	m_mediaScanner->start();
	CMsgOpenDb* dbmsg = new CMsgOpenDb( getProperty("stateDBfile", "state.db") );
	m_mediaScanner->sendMsg(dbmsg);
	addOutstandingMsg(dbmsg);

	CMsgScanDir* sdmsg = new CMsgScanDir("/home/martin/Desktop");
	m_mediaScanner->sendMsg(sdmsg);
	addOutstandingMsg(sdmsg);

	setClientCmdIdBySubprocessCmdID(sdmsg->getID(), initiator, jobID );
}

void CSession::scanProgress(uint32_t jobID, uint32_t progress) {
	client_job_t cljob;
	try {
		cljob = getClientCmdIdBySubprocessCmdID(jobID, false);

		CmdProgress* progCmd = new CmdProgress(cljob.second, progress);
		cljob.first->sendCmd(progCmd);
	}catch(InvalidMsgException ex) {

	}
}

void CSession::jobFinished(uint32_t jobID) {
	client_job_t clientJob;
	try {
		clientJob = getClientCmdIdBySubprocessCmdID(jobID);

		CmdFinished* finiCmd = new CmdFinished(clientJob.second);
		clientJob.first->sendCmd(finiCmd);

	}catch(InvalidMsgException ex) {
		;
	}

	delOutstandingMsg(jobID);
}

void CSession::collectionChanged(uint32_t newRev, uint32_t minRev, uint32_t maxRev) {
	CRootItem* newMediaColRev = m_stateDB->getMediaColRev(newRev);
	addMediaColRev(newMediaColRev);

	assert(newRev == m_maxMediaColRev);
}

void CSession::response(uint32_t requestID, int32_t returnCode, string message) {
	delOutstandingMsg(requestID);

}

void CSession::reportError(uint32_t jobID, int32_t errCode, string message) {
	delOutstandingMsg(jobID);

}

void CSession::toAll( Cmd* cmd ) {
	set<CConnection*>::iterator it = m_connections.begin();
	while( it != m_connections.end() ) {
		(*it)->sendCmd(cmd);
		++it;
	}
	delete cmd;
}

string CSession::getProperty(string key, string defaultVal) {
	string privKey = privatePropertyKey(key);
	string value = m_app->settings().getProptery(privKey, defaultVal);
	return value;
}

void CSession::setProperty(string key, string val) {
	string privKey = privatePropertyKey(key);
	m_app->settings().setProptery(privKey, val);
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

void CSession::delOutstandingMsg(uint32_t id) throw(InvalidMsgException) {
	map<uint32_t, CMsgBase*>::iterator it = m_outstanding_msgs.find(id);
	if(it == m_outstanding_msgs.end() ) {
		ostringstream oss;
		oss << "no command with id '" << id << "' to childprocess outstanding.";
		throw InvalidMsgException(oss.str());
	}
	CMsgBase* msg = it->second;

	// cancel timer here

	m_outstanding_msgs.erase(it);
}

muroa::CSession::client_job_t CSession::getClientCmdIdBySubprocessCmdID(uint32_t subprocess_cmd_id, bool delentry) throw(InvalidMsgException) {
	map<uint32_t, client_job_t>::iterator it = m_subprocess_job_by_cmdID.find(subprocess_cmd_id);
	if(it == m_subprocess_job_by_cmdID.end()) {
		ostringstream oss;
		oss << "no client is waiting for an answer to subprocess command '" << subprocess_cmd_id << "'";
		throw InvalidMsgException( oss.str() );
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

} /* namespace muroa */
