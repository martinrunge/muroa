/*
 * CSession.h
 *
 *  Created on: 11 Feb 2012
 *      Author: martin
 */

#ifndef CSESSION_H_
#define CSESSION_H_

#include <qobject.h>

#include "CConnection.h"
#include "CServiceDesc.h"
#include "CDnsSdServiceBrowserAvahi.h"

class CRootItem;
class CMuroaTreeModel;
class CMuroaListModel;

class CSession: public QObject {
    Q_OBJECT
public:
	CSession();
	virtual ~CSession();

    CMuroaTreeModel *getMediaColModel() const;
    CMuroaListModel *getNextlistModel() const;
    CMuroaListModel *getPlaylistModel() const;

    const CConnection* getConnection() const { return &m_connection; };

public slots:
    void openConnection(const CServiceDesc & sd);
    void closeConnection();
    void scanCollection();

    uint32_t getMediaColRev() const;
    uint32_t getNextlistRev() const;
    uint32_t getPlaylistRev() const;

    void setMediaColRev(uint32_t mediaColRev);
    void setNextlistRev(uint32_t nextlistRev);
    void setPlaylistRev(uint32_t playlistRev);

private:
    CConnection m_connection;
    CMuroaTreeModel *m_mediaColModel;
    CMuroaListModel *m_playlistModel;
    CMuroaListModel *m_nextlistModel;

    uint32_t m_mediaColRev;
    uint32_t m_playlistRev;
    uint32_t m_nextlistRev;
};

#endif /* CSESSION_H_ */
