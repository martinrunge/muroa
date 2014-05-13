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
#include <libavresample/avresample.h>
}

using namespace std;

#include <string>
#include <stdint.h>
#include <assert.h>

#include <thread>


class CStream;

class CDecoder {
public:
	CDecoder(const CStream *streamPtr = 0);
	virtual ~CDecoder();

	void open(std::string filename);
	void close();
	inline bool isOpen() const { return m_open;};
	inline std::string getCurrentFilename() { return m_filename; };

	int decode();

	int getDuration();
	void decodingLoop();


private:
	std::string m_filename;
	const CStream *m_streamPtr;

	AVFormatContext *m_pFormatCtx;
    AVCodecContext *m_pCodecCtx;
    AVCodec *m_pCodec;
//    AVAudioResampleContext *m_pResamplerCtx;

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

	bool m_open;

	uint8_t* m_frame_buffer;
	int  m_frame_buffer_size;

	uint8_t* m_res_frame_buffer;
	int m_res_frame_buffer_size;
};

#endif /* CDECODER_H_ */
