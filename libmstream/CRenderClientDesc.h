/*
 * CRenderClientDesc.h
 *
 *  Created on: 16.05.2016
 *      Author: martin
 */

#ifndef LIBMSTREAM_CRENDERCLIENTDESC_H_
#define LIBMSTREAM_CRENDERCLIENTDESC_H_

#include <string>
#include "avahi/CServiceDesc.h"

namespace muroa {

class CRenderClientDesc {
public:
	CRenderClientDesc();
	virtual ~CRenderClientDesc();

	const bool& isOnline() const { return m_is_online; }
	void isOnline(const bool& online) { m_is_online = online; }

	const bool& isMember() const { return m_is_member; }
	void isMember(const bool& is_member) { m_is_member = is_member; }

	const std::string& getMemberOfSession() const {	return m_member_of_session; }
	void setMemberOfSession(const std::string& memberOfSession) { m_member_of_session = memberOfSession; }

	ServDescPtr srvPtr;

private:
	bool m_is_online;
	bool m_is_member;
	std::string m_member_of_session;

};

} /* namespace muroa */

#endif /* LIBMSTREAM_CRENDERCLIENTDESC_H_ */
