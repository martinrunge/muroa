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

enum cmd_type {
	E_PLAY,
	E_STOP,
	E_NEXT,
	E_RPEV,

};


class CCmdBase {
public:
	virtual ~CCmdBase();

	inline std::string name() const { return m_name; };
	inline void setName(std::string name) { m_name = name; };

	inline int knownRev() { return m_known_rev; };

	void send();
	void ack();

	int id() { return m_id; };

	virtual void timeout();

protected:
	CCmdBase(enum cmd_type type);
	std::string m_name;

	int m_known_rev;
	enum cmd_type m_type;

private:
	const long long m_id;

	static int m_timeout_in_ms;
	static long long m_first_free_id;
};

} /* namespace muroa */
#endif /* CCMDBASE_H_ */
