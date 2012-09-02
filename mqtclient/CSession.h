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

#include "CMuroaTreeModel.h"
#include "CMuroaListModel.h"

class CRootItem;

class CSession: public QObject {
    Q_OBJECT
public:
	CSession();
	virtual ~CSession();

    CMuroaTreeModel *getMediaColModel() const { return m_mediaColModel; };
    CMuroaListModel *getNextlistModel() const { return m_playlistModel; };
    CMuroaListModel *getPlaylistModel() const { return m_nextlistModel; };
    CMuroaTreeModel *getSessionStateModel() const { return m_sessionStateModel; };

    const CConnection* getConnection() const { return &m_connection; };

    void save();
    void restore();

public slots:
    void openConnection(const CServiceDesc & sd);
    void closeConnection();
    void scanCollection();
    void dumpCollection();

    inline uint32_t getMediaColRev() const { return m_mediaColModel->getRevision(); };
    inline uint32_t getNextlistRev() const { return m_playlistModel->getRevision(); };
    inline uint32_t getPlaylistRev() const { return m_nextlistModel->getRevision(); };
    inline uint32_t getSessionStateRev() const { return m_sessionStateModel->getRevision(); };
//
//    void setMediaColRev(uint32_t mediaColRev);
//    void setNextlistRev(uint32_t nextlistRev);
//    void setPlaylistRev(uint32_t playlistRev);

private:
    CConnection m_connection;
    CMuroaTreeModel *m_mediaColModel;
    CMuroaListModel *m_playlistModel;
    CMuroaListModel *m_nextlistModel;
    CMuroaTreeModel *m_sessionStateModel;
//    uint32_t m_mediaColRev;
//    uint32_t m_playlistRev;
//    uint32_t m_nextlistRev;
    QString m_storeageLoc;
};

#endif /* CSESSION_H_ */
