#ifndef CHWTEST_H
#define CHWTEST_H

#include <QObject>
#include "cnetwork.h"
#include "ccontenthandler.h"
#include "cstatemachine.h"
#include "CSession.h"

class QXmlInputSource;
class QXmlSimpleReader;
class QIODevice;
//class CCollection;
// class CSession;

class CConnection : public QObject
{
    Q_OBJECT;
public:
    CConnection();
    ~CConnection();

signals:
    void connectionStatusChanged(QString message);

public slots:
    void newConnection(QIODevice* ioDev);
    void connectionClosed(QIODevice* ioDev);

    void readyRead();

//    inline void setCollection(CCollection* collection) { m_session->setCollectionPtr(collection); };
    inline void setSessionPtr(CSession * session)
    {
    	m_session = session;
    	m_sm.setSessionPtr(m_session);
    };

private:
    CNetwork *m_net;
    QIODevice* m_io_dev;
    QXmlStreamReader* m_xml_reader;
    QXmlStreamWriter* m_xml_writer;

    CStateMachine m_sm;

    CContentHandler m_contentHandler;
    CSession* m_session;
};

#endif // CHWTEST_H
