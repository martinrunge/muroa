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
		STOP,
		NEXT,
		PREV,
		ERROR,
		FINI,
		PROGRESS,
		OPENDB,
		RESP,
		COLCHANGED,
		PLCHANGED,
		NLCHANGED,
	};
	typedef enum type_t type_t;

	virtual ~Cmd();

	inline std::string name() const { return m_name; };
	inline void setName(std::string name) { m_name = name; };

	inline int knownRev() { return m_known_rev; };

	void send();
	void ack();

	int id() { return m_id; };

	virtual void timeout();
	Cmd::type_t type() { return m_type; };

protected:
	Cmd(type_t type);
	std::string m_name;

	int m_known_rev;
	type_t m_type;

private:
	const long long m_id;

	static int m_timeout_in_ms;
	static long long m_first_free_id;
};

} /* namespace muroa */
#endif /* CCMDBASE_H_ */
