/*
 * CStream.h
 *
 *  Created on: 13 Jun 2010
 *      Author: martin
 */

#ifndef CSTREAM_H_
#define CSTREAM_H_

#include <QObject>

#include "CDecoder.h"

class CCollectionItem;
class QTimer;
class IAudioIO;

enum sessionState {
	e_stopped = 0,
	e_playing = 1,
	e_paused = 2,
	e_none,
};


class CStream : public QObject {
	Q_OBJECT;
public:
	CStream();
	virtual ~CStream();

	void next() const;
	int write(char* data, int size)  const;

signals:
	void finished() const;
	void progress(int done, int total);

public slots:
	void setSong(CCollectionItem* item);
	void play();
	void stop();

	void timeout();


private:
	void start();

	enum sessionState m_state;
	QString m_fileName;
	QTimer *m_timer;

	int m_done;
	int m_total;

	CDecoder m_decoder;
	IAudioIO* m_audioIO;


};

#endif /* CSTREAM_H_ */
