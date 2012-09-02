/*
 * CConnection.h
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#ifndef CCONNECTION_H_
#define CCONNECTION_H_

#include <CTcpConnection.h>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>



namespace muroa {

class CRpc;
class CSession;
class CSessionContainer;

class Cmd;
class CmdPlay;
class CmdStop;
class CmdNext;
class CmdPrev;
class CmdProgress;
class CmdFinished;
class CmdEditMediaCol;
class CmdResp;
class CmdError;


class CConnection : public CTcpConnection {
public:
	virtual ~CConnection();

	inline unsigned getID() { return m_id; };
	void start();
	void stop();

	static CConnection* create(boost::asio::io_service& io_service) {
	    return new CConnection(io_service);
	}

	void onJoinSession(std::string name);
	void joinSession(std::string name);

	void sendLatestMediaColRev(uint32_t jobID, unsigned knownRev = 0);
	void sendLatestPlaylistRev(uint32_t jobID, unsigned knownRev = 0);
	void sendLatestNextlistRev(uint32_t jobID, unsigned knownRev = 0);
	void sendLatestSessionStateRev(uint32_t jobID, unsigned knownRev = 0);

	void dataReceived( boost::array<char, 8192> buffer, int length);

    CSession *getSession() const;
    void setSession(CSession *session);

    void sendCmd( Cmd* cmd );
    void incomingCmd( Cmd* cmd );

private:
    CConnection(boost::asio::io_service& io_service);

    void onClose();

    CRpc *m_rpc;
    CSession* m_session;
    CSessionContainer* m_session_container;

    const uint32_t m_id;
    static unsigned m_next_id;

    void play(CmdPlay* playCmd);
    void stop(CmdStop* stopCmd);
    void next(CmdNext* nextCmd);
    void prev(CmdPrev* prevCmd);
    void progress(CmdProgress* progressMsg);
    void finished(CmdFinished* finishedCmd);
    void collectionChanged(CmdEditMediaCol* colChangedCmd);
    void response(CmdResp* respCmd);
    void error(CmdError* errorMsg);
};

} /* namespace muroa */
#endif /* CCONNECTION_H_ */
