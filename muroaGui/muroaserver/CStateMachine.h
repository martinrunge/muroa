#ifndef CSTATEMACHINE_H
#define CSTATEMACHINE_H

#include <QObject>

class QTcpSocket;
class QXmlStreamReader;
class QXmlStreamWriter;
// class CCollection<temlate class T>;
class CConnection;
class CSession;

enum states { e_not_connected,
			  e_connected,
			  e_next_requested,
			  e_prev_requested,
			  e_play_requested,
			  e_stop_requested,
			  e_collection_requested,
			  e_playlist_requested,
			  e_nextlist_requested,
			  e_awaiting_collection_mod,
			  e_awaiting_playlist_mod,
			  e_awaiting_nextlist_mod,
			};


class CStateMachine : public QObject
{
    Q_OBJECT;
public:
    CStateMachine(CConnection* connection);

    ~CStateMachine();
    inline void setXmlWriter(QXmlStreamWriter* xml_writer) { m_xml_writer = xml_writer; };

public slots:
    void close();
    void open();

    void addData(QByteArray data);

    void startDocument(QXmlStreamReader* reader);
    void endDocument(QXmlStreamReader* reader);

    void startElement(QXmlStreamReader* reader);
    void endElement(QXmlStreamReader* reader);
    void characters(QXmlStreamReader* reader);

    // inline void setCollection(CCollection* collection) { m_collection = collection; };
    inline void setSessionPtr(CSession* session) { m_session = session; };

    // void sendCollection();

private:
    int m_state;

    QXmlStreamReader* m_xml_reader;
    int m_xml_depth;

    // int m_revision;
    int m_knownRevision;
    int m_diffFromRev;

    QString m_nextlistDiff;
    QString m_playlistDiff;
    QString m_collectionDiff;

    CConnection* m_connection;
    CSession* m_session;

    QXmlStreamWriter* m_xml_writer;

    void parseNextArgs(QXmlStreamReader* reader);
    void parsePrevArgs(QXmlStreamReader* reader);
    void parseStopArgs(QXmlStreamReader* reader);
    void parsePlayArgs(QXmlStreamReader* reader);

    void parseGetNextlistArgs(QXmlStreamReader* reader);
    void parseGetPlaylistArgs(QXmlStreamReader* reader);
    void parseGetCollectionArgs(QXmlStreamReader* reader);

    void parseModNextlistArgs(QXmlStreamReader* reader);
    void parseModPlaylistArgs(QXmlStreamReader* reader);
    void parseModCollectionArgs(QXmlStreamReader* reader);
};

#endif // CSTATEMACHINE_H
