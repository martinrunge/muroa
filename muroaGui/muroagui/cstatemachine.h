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

enum states { e_no_session,
			  e_session_active,
			  e_awaiting_collection,
			  e_reading_collection,
			  e_collection_received,
			  e_reading_playlist,
			  e_playlist_received  };

class CStateMachine : public QObject
{
    Q_OBJECT;
public:
    CStateMachine();
    ~CStateMachine();

    inline void setXmlWriter(QXmlStreamWriter* const xml_writer) { m_xml_writer = xml_writer; };
    inline void setPlaylistModelPtr(CPlaylistModel* const playlistModelPtr ) { m_playlistModelPtr = playlistModelPtr; };
    inline void setCollectionModelPtr(CCollectionModel* const collectionModelPtr ) { m_collectionModelPtr = collectionModelPtr; };

    inline int getRevision() { return m_revision; };

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
    int m_xml_depth;

    CPlaylistModel* m_playlistModelPtr;
    CCollectionModel* m_collectionModelPtr;

    int m_revision;
    int m_diffFromRev;

    QXmlStreamWriter* m_xml_writer;

    void parsePlaylistArgs(QXmlStreamReader* reader);
    void parseCollectionArgs(QXmlStreamReader* reader);

    template <typename T> void parseCollection(QStringRef text, CModelBase<T>* model);
    template <typename T> void parseCollectionDiff(QStringRef text, CModelBase<T>* model);

    void parseReadArgs(QXmlStreamReader* reader);
    void parseWriteArgs(QXmlStreamReader* reader);

};

#endif // CCONTENTHANDLER_H
