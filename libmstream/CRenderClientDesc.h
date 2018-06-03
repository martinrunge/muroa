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
#include "CTime.h"

namespace muroa {

class CRenderClientDesc {
public:
	CRenderClientDesc();
	virtual ~CRenderClientDesc();

	const bool& isOnline() const { return m_is_online; }
	void isOnline(const bool& online) { m_is_online = online; }

	const bool& isMember() const { return m_is_member; }
	void isMember(const bool& is_member) { m_is_member = is_member; }

	const int& getVolume() const { return m_volume; }
	void setVolume(const int& volume) { m_volume = volume; }

	const std::string& getMemberOfSession() const {	return m_member_of_session; }
	void setMemberOfSession(const std::string& memberOfSession) { m_member_of_session = memberOfSession; }

	const std::string& getLastErrorMsg() const { return m_last_error_msg; }
	void setLastErrorMsg(const std::string& lastErrorMsg) {	m_last_error_msg = lastErrorMsg; }

	const CTime& getLastSyncError() const {	return m_last_sync_error; }
	void setLastSyncError(const CTime& lastSyncError) {	m_last_sync_error = lastSyncError; }

	ServDescPtr srvPtr;

private:
	bool m_is_online;
	bool m_is_member;
	int m_volume;
	std::string m_member_of_session;
	std::string m_last_error_msg;
	CTime m_last_sync_error;
};

} /* namespace muroa */

#endif /* LIBMSTREAM_CRENDERCLIENTDESC_H_ */
