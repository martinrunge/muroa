/*
 * CConnection.h
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#ifndef CCONNECTION_H_
#define CCONNECTION_H_

#include <CTcpConnection.h>

namespace muroa {

class CRpc;

class CConnection : public CTcpConnection {
public:
	virtual ~CConnection();

	typedef boost::shared_ptr<CConnection> pointer;

	static pointer create(boost::asio::io_service& io_service) {
	    return pointer(new CConnection(io_service));
	}

    void dataReceived( boost::array<char, 8192> buffer, int length);

private:
    CConnection(boost::asio::io_service& io_service);

    CRpc *m_rpc;
};

} /* namespace muroa */
#endif /* CCONNECTION_H_ */
