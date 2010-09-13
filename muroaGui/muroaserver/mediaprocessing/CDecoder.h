/*
 * CDecoder.h
 *
 *  Created on: 19 Jun 2010
 *      Author: martin
 */

#ifndef CDECODER_H_
#define CDECODER_H_

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#include <string>
#include <stdint.h>
#include <assert.h>

#include <thread>

using namespace std;

class CStream;

class CDecoder {
public:
	CDecoder(const CStream *streamPtr = 0);
	virtual ~CDecoder();

	void open(const char* filename);
	void close();
	inline bool isOpen() const { return m_open;};

	int decode();

	int getDuration();
	void decodingLoop();


private:
	const CStream *m_streamPtr;

	AVFormatContext *m_pFormatCtx;
    AVCodecContext *m_pCodecCtx;
    AVCodec *m_pCodec;

    // struct AVPacket: defined in avformat.h:
	// most important attributes in the strcture:
	//
	// packet.data     :the packets payload
	// packet.length   :length of payload in bytes
	// packet.pts      :presentation timestamp in units of time_base. see above.
	// packet.duration :packet length in time_base unit
	// packet.dts      :decoding timestamp
 	AVPacket m_packet;

	// The file may contain more than on stream. Each stream can be a
	// video stream, an audio stream, a subtitle stream or something else.
	// also see 'enum CodecType' defined in avcodec.h
	// Find the first audio stream:
	int m_audioStreamID;

	AVRational m_timeBase;
	int        m_durationInSecs;
	int        m_posInSecs;


	std::thread *m_thread;
	bool m_stop;
	bool m_open;

};

#endif /* CDECODER_H_ */
