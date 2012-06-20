/*
 * CCommand.h
 *
 *  Created on: 17 Jan 2010
 *      Author: martin
 */

#ifndef CCOMMANDBASE_H_
#define CCOMMANDBASE_H_

#include <string>

#include <QObject>
#include <QTimer>

class CmdBase : public QObject
{
	Q_OBJECT;
public:

	enum cmdType {
		GET_MEDIA_COL,
		GET_PLAYLIST,
		GET_NEXTLIST,
		EDIT_MEDIA_COL,
		EDIT_PLAYLIST,
		EDIT_NEXTLIST,
		PLAY,
		PAUSE,
		NEXT,
		PREV,
		SCAN_COLLECTION,
	};
	typedef cmdType Type;

	CmdBase(Type cmdType, int knownRev, QObject * parent = 0 );
	virtual ~CmdBase();

	inline std::string name() const { return m_name; };
	inline void setName(std::string name) { m_name = name; };
	inline std::string data() const { return m_data; };
	inline void setData(std::string data) { m_data = data; };

	inline const CmdBase::Type type() const { return m_type; };

	inline int knownRev() { return m_known_rev; };

	void send();
	void ack();

	unsigned id() { return m_Id; };

	inline static unsigned getNextID() { return m_first_free_id++; };

public slots:
	virtual void timeout();

protected:
	std::string m_name;
	std::string m_data;

	unsigned m_known_rev;
	enum cmdType m_type;

private:
	QTimer m_timer;
	unsigned m_Id;

	static int m_timeout_in_ms;
	static unsigned m_first_free_id;
};

#endif /* CCOMMANDBASE_H_ */
