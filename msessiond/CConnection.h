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

class CConnection : public boost::enable_shared_from_this<CConnection>, public CTcpConnection {
public:
	virtual ~CConnection();

	typedef boost::shared_ptr<CConnection> pointer;

	static pointer create(boost::asio::io_service& io_service) {
	    return pointer(new CConnection(io_service));
	}

	void joinSession(std::string name);

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
