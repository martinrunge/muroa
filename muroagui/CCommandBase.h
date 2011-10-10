/*
 * CCommand.h
 *
 *  Created on: 17 Jan 2010
 *      Author: martin
 */

#ifndef CCOMMANDBASE_H_
#define CCOMMANDBASE_H_

#include <QObject>
#include <QTimer>


class CCommandBase : public QObject
{
	Q_OBJECT;
public:
	CCommandBase(int knownRev, QObject * parent = 0 );
	virtual ~CCommandBase();

	inline QString commandName() const { return m_cmdName; };
	inline void setCommandName(QString name) { m_cmdName = name; };
	inline QString commandData() const { return m_cmdData; };
	inline void setCommandData(QString cmdData) { m_cmdData = cmdData; };

	inline int knownRev() { return m_known_rev; };

	void send();
	void ack();

	int id() { return m_cmdId; };

public slots:
	virtual void timeout();

protected:
	QString m_cmdName;
	QString m_cmdData;

	int m_known_rev;

private:
	QTimer m_timer;
	int m_cmdId;

	static int m_timeout_in_ms;
	static int m_first_free_id;
};

#endif /* CCOMMANDBASE_H_ */
