/*
 * CDecoder.cpp
 *
 *  Created on: 19 Jun 2010
 *      Author: martin
 */

#include "CDecoder.h"
#include <iostream>
#include <string>

#include "CStream.h"

CDecoder::CDecoder(const CStream* streamPtr) : m_streamPtr(streamPtr),
                                               m_pFormatCtx(0),
                                               m_pCodecCtx(0),
                                               m_pCodec(0),
                                               m_thread(0),
                                               m_open(false) {

	// This registers all available file formats and codecs with the library so
    // they will be used automatically when a file with the corresponding format/codec
    // is opened. Note that you only need to call av_register_all() once, so it's
    // probably best to do this somewhere in your startup code. If you like, it's
    // possible to register only certain individual file formats and codecs, but
    // there's usually no reason why you would have to do that.
	av_register_all();
	av_init_packet(&m_packet);
}

CDecoder::~CDecoder() {
	if(m_open) close();
}


void CDecoder::open(const char* filename)
{
	if(m_open) {
		cerr << "Warning: CDecoder::open called while decoder was still open." << endl
			 << "Decoder can be opened only once. Call CDecoder::close() before." << endl;
		return;
	}

	av_init_packet(&m_packet);

	// The last three parameters specify the file format, buffer size and format parameters;
	// by simply specifying NULL or 0 we ask libavformat to auto-detect the format and
	// use a default buffer size.
	if(av_open_input_file(&m_pFormatCtx, filename, NULL, 0, NULL) != 0 )
		cout << " Couldn't open file " << filename << endl;

	// Next, we need to retrieve information about the streams contained in the file:
	if(av_find_stream_info(m_pFormatCtx) < 0 )
		cerr << "Couldn't find stream information in file " << filename << endl;

	// dump_format(m_pFormatCtx, 0, filename, false);


	m_audioStreamID = -1;

	for(unsigned i=0; i < m_pFormatCtx->nb_streams; i++) {
		if(m_pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_AUDIO) {
			m_audioStreamID = i;
			break;
		}
	}
	if(m_audioStreamID == -1) {
		cerr << "Didn't find a audio stream in file " << filename << endl;
		return;
	}

	m_pCodecCtx = m_pFormatCtx->streams[m_audioStreamID]->codec;


	// get the timebase for this stream. The presentation timestamp,
	// decoding timestamp and packet duration is expressed in timestamp
	// as unit:
	// e.g. if timebase is 1/90000, a packet with duration 4500
	// is 4500 * 1/90000 seconds long, that is 0.05 seconds == 20 ms.
	m_timeBase = m_pFormatCtx->streams[m_audioStreamID]->time_base;

	// duration in m_timBase
	int64_t tmp = m_pFormatCtx->streams[m_audioStreamID]->duration * m_timeBase.num;
	m_durationInSecs = tmp / m_timeBase.den;

	m_open = true;
}

void CDecoder::close()
{
	// cerr << "CDecoder::close" << endl;

	m_stop = true;
	if(m_thread != 0) {
		m_thread->join();
		delete m_thread;
		m_thread = 0;
	}

    // Free old packet
    if(m_packet.data != NULL) {
        av_free_packet(&m_packet);
        m_packet.data = NULL;
    }
    // close the Codec
    if(m_pCodecCtx->codec != 0) {
    	avcodec_close(m_pCodecCtx);
    	m_pCodecCtx->codec = 0;
    	m_pCodecCtx=0;
    }

    // Close the video file
    if(m_pFormatCtx != 0 ) {
    	av_close_input_file(m_pFormatCtx);
    	m_pFormatCtx = 0;
    }
    m_open = false;
}

int CDecoder::decode() {
	m_posInSecs = 0;
	if(m_streamPtr != 0) {
		// set pos to 0 to ensure gui client see 0 seconds progress so they can set title, artist, etc of new song in the gui.
		m_streamPtr->setProgress( m_posInSecs, m_durationInSecs );
	}

	m_pCodec=avcodec_find_decoder(m_pCodecCtx->codec_id);
	if(m_pCodec==NULL)
		cerr << "Codec type " <<  m_pCodecCtx->codec_id << " not found." << endl;

	// Open the codec found suitable for this stream in the last step
	if(avcodec_open(m_pCodecCtx, m_pCodec) < 0)
		cerr << " Could not open codec." << endl;

	m_stop = false;
	m_open = true;

	m_thread = new thread( &CDecoder::decodingLoop, this);

	return 0;
}


void CDecoder::decodingLoop()
{
    // prepare a buffer to store the decoded samples in
    const int sampleBufSize = 2 * AVCODEC_MAX_AUDIO_FRAME_SIZE;
    static int16_t sampleBuffer[sampleBufSize];

 	bool end_of_stream = false;
 	do {
        // read and forget packets until a packet with the
        // right stream ID (audioStreamID defined above) is found.
 		do {
 	   		// Free old packet
       		if(m_packet.data != NULL) {
    	   		av_free_packet(&m_packet);
    	   		m_packet.data = NULL;
       		}

       		// Read new frame
       		if(av_read_frame(m_pFormatCtx, &m_packet) < 0) {
       			end_of_stream = true;
 	      		break;
       		}
    	} while(m_packet.stream_index != m_audioStreamID);
    	// here, a new audio packet from the stream is available

        // cerr << "packetsize = " << m_packet.size << endl;

        // to simulate a network connection, pass just the packet itself, the codecID
        // and the presentation time to the receiver. This is the same information like
        // a RTP connection can transport.
        // sendPacket(m_pCodecCtx->codec_id, (uint8_t*)m_packet.data, m_packet.size, m_packet.pts, m_timeBase);

        // when calling avcodec_decode_audio2, sb hold the sample buffer size,
        // on return the number of produced samples is stored here. bytes_used
        // indicates how many bytes of the data was used for decoding. When
        // provided with a self contained packet, it should be used completely.
        int sb = sampleBufSize;
        int bytesUsed;
        bytesUsed = avcodec_decode_audio3(m_pCodecCtx, sampleBuffer, &sb, &m_packet);

        // only call m_streamPtr->setProgress every second
        int64_t tmp = m_packet.pts * m_timeBase.num;
        int playedInSecs = tmp / m_timeBase.den;

        if( playedInSecs > m_posInSecs) {
        	m_posInSecs = playedInSecs;
        	m_streamPtr->setProgress( m_posInSecs, m_durationInSecs );
        }

        // write the decoded data to the output file.
        // fwrite(sampleBuffer, sb, 1, outfile);
        m_streamPtr->write((char*)sampleBuffer, sb);


 	} while(!end_of_stream && !m_stop );

	if(! m_stop ) {
		m_streamPtr->next();
	}
}


int CDecoder::getDuration() {
	return m_durationInSecs;
}

