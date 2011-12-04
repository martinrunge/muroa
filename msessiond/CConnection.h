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

class CConnection : public CTcpConnection {
public:
	virtual ~CConnection();

	void start();
	void stop();

	static CConnection* create(boost::asio::io_service& io_service) {
	    return new CConnection(io_service);
	}

	void joinSession(std::string name);

	void sendLatestMediaColRev(unsigned knownRev = 0);
	void sendLatestPlaylistRev(unsigned knownRev = 0);
	void sendLatestNextlistRev(unsigned knownRev = 0);

	void dataReceived( boost::array<char, 8192> buffer, int length);

    CSession *getSession() const;
    void setSession(CSession *session);

private:
    CConnection(boost::asio::io_service& io_service);

    CRpc *m_rpc;
    CSession* m_session;
    CSessionContainer* m_session_container;
};

} /* namespace muroa */
#endif /* CCONNECTION_H_ */
