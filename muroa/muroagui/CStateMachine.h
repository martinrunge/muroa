#ifndef CCONTENTHANDLER_H
#define CCONTENTHANDLER_H


#include <QString>
#include <QObject>
#include <QAbstractTableModel>

#include "CModelBase.h"

class QTcpSocket;
class QXmlStreamReader;
class QXmlStreamWriter;
class CPlaylistModel;
class CCollectionModel;

enum sessionState {
	e_stopped = 0,
	e_playing = 1,
	e_paused = 2,
	e_none,
};

enum states { e_no_session,
			  e_session_active,
			  e_awaiting_collection,
			  e_reading_collection,
			  e_collection_received,
			  e_reading_nextlist,
			  e_nextlist_received,
			  e_reading_playlist,
			  e_playlist_received,
              e_progress,
			  e_state_changed };

class CStateMachine : public QObject
{
    Q_OBJECT;
public:
    CStateMachine();
    ~CStateMachine();

    inline void setXmlWriter(QXmlStreamWriter* const xml_writer) { m_xml_writer = xml_writer; };
    inline void setNextlistModelPtr(CPlaylistModel* const nextlistModelPtr ) { m_nextlistModelPtr = nextlistModelPtr; };
    inline void setPlaylistModelPtr(CPlaylistModel* const playlistModelPtr ) { m_playlistModelPtr = playlistModelPtr; };
    inline void setCollectionModelPtr(CCollectionModel* const collectionModelPtr ) { m_collectionModelPtr = collectionModelPtr; };

    inline int getRevision() { return m_revision; };

signals:
    void progress( int done, int total );
    void stateChanged( int sessionState );

public slots:
    void close();
    void open();

    void startDocument(QXmlStreamReader* reader);
    void endDocument(QXmlStreamReader* reader);

    void startElement(QXmlStreamReader* reader);
    void endElement(QXmlStreamReader* reader);
    void characters(QXmlStreamReader* reader);


private:
    int m_state;
    int m_sessionState;
    int m_xml_depth;

    CPlaylistModel* m_playlistModelPtr;
    CPlaylistModel* m_nextlistModelPtr;
    CCollectionModel* m_collectionModelPtr;

    int m_revision;
    int m_diffFromRev;

    int m_done;
    int m_total;

    QXmlStreamWriter* m_xml_writer;

    void parseProgressArgs(QXmlStreamReader* reader);
    void parseStateChangedArgs(QXmlStreamReader* reader);

    void parseNextlistArgs(QXmlStreamReader* reader);
    void parsePlaylistArgs(QXmlStreamReader* reader);
    void parseCollectionArgs(QXmlStreamReader* reader);

    template <typename T> void parseCollection(QStringRef text, CModelBase<T*>* model);
    template <typename T> void parseCollectionDiff(QStringRef text, CModelBase<T*>* model);

    void parseReadArgs(QXmlStreamReader* reader);
    void parseWriteArgs(QXmlStreamReader* reader);

};

#endif // CCONTENTHANDLER_H
