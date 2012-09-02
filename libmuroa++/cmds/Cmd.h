/*
 * CCmdBase.h
 *
 *  Created on: 31 Oct 2011
 *      Author: martin
 */

#ifndef CCMDBASE_H_
#define CCMDBASE_H_

#include <string>

namespace muroa {

class Cmd {
public:

	enum type_t {
		PLAY,
		PAUSE,
		STOP,
		NEXT,
		PREV,
		ERROR,
		FINISHED,
		PROGRESS,
		OPENDB,
		RESP,
		EDIT_MEDIACOL,
		EDIT_PLAYLIST,
		EDIT_NEXTLIST,
		EDIT_SESSION_STATE,
		ADD_UNASSIGNED_CLIENT,
		RM_UNASSIGNED_CLIENT,
		ADD_CLIENT_TO_SESSION,
		RM_CLIENT_FROM_SESSION,
	};
	typedef enum type_t type_t;

	virtual ~Cmd();

	inline std::string name() const { return m_name; };
	inline void setName(std::string name) { m_name = name; };

	inline int knownRev() { return m_known_rev; };

	void send();
	void ack();

	int id() { return m_id; };

	inline unsigned getConnectionID() { return m_connection_id; };
	inline void setConnectionID(unsigned connID) { m_connection_id = connID; };

	virtual void timeout();
	Cmd::type_t type() { return m_type; };

protected:
	Cmd(type_t type);
	std::string m_name;

	int m_known_rev;
	type_t m_type;

	// an id to keep track of the connection over which this command was originally received.
	// useful to send error messages to sender only.
	unsigned m_connection_id;

private:
	const long long m_id;

	static int m_timeout_in_ms;
	static long long m_first_free_id;
};

} /* namespace muroa */
#endif /* CCMDBASE_H_ */
