/*
 * CSessionSM.h
 *
 *  Created on: 28 Nov 2011
 *      Author: martin
 */

#ifndef CCLIENTSM_H_
#define CCLIENTSM_H_

#include <map>
#include <QObject>

class CConnection;
class CmdBase;

class CClientSM : public QObject {
	Q_OBJECT
public:
	CClientSM(CConnection* connection, QObject* parent = 0);
	virtual ~CClientSM();

    void getLatestMediaCol();
    void getLatestPlaylist();
    void getLatestNextlist();

public slots:
	void scanCollection();


private:
    CConnection* m_connection;

    std::map<unsigned, CmdBase*> m_open_cmds;
};

#endif /* CCLIENTSM_H_ */
