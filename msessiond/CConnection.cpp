/*
 * CConnection.cpp
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#include "CConnection.h"
#include "CRpc.h"

namespace muroa {

CConnection::~CConnection() {
	// TODO Auto-generated destructor stub
}

CConnection::CConnection(boost::asio::io_service& io_service) : CTcpConnection(io_service){
	m_rpc = new CRpc();
}

void CConnection::dataReceived( boost::array<char, 8192> buffer, int length) {
	m_rpc->newData(buffer.data(), length);
}

} /* namespace muroa */
