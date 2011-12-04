/*
 * CSessionSM.h
 *
 *  Created on: 28 Nov 2011
 *      Author: martin
 */

#ifndef CSESSIONSM_H_
#define CSESSIONSM_H_

#include <map>
#include <QObject>

class CConnection;
class CmdBase;

class CSessionSM : public QObject {
	Q_OBJECT
public:
	CSessionSM(CConnection* connection, QObject* parent = 0);
	virtual ~CSessionSM();

    void getLatestMediaCol();
    void getLatestPlaylist();
    void getLatestNextlist();

    unsigned getKnownMediaColRev() const;
    unsigned getKnownNextlistRev() const;
    unsigned getKnownPlaylistRev() const;

public slots:
	void scanCollection();


private:
    CConnection* m_connection;

    std::map<unsigned, CmdBase*> m_open_cmds;

    unsigned m_knownMediaColRev;
    unsigned m_knownPlaylistRev;
    unsigned m_knownNextlistRev;

};

#endif /* CSESSIONSM_H_ */
