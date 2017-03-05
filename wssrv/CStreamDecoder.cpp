/*
 * CStreamDecoder.cpp
 *
 *  Created on: 19 Jun 2010
 *      Author: martin
 */

#include <iostream>
#include <string>
#include <thread>

#include "CppServer.h"

#include <libavutil/opt.h>
#include "CStreamDecoder.h"

#include <log4cplus/loggingmacros.h>

#include <CApp.h>

#ifndef AVCODEC_MAX_AUDIO_FRAME_SIZE
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio
#endif

using namespace std;
using namespace muroa;

CStreamDecoder::CStreamDecoder(CppServer *streamSrvPtr) : m_streamSrvPtr(streamSrvPtr),
		                                       m_play_thread(0),
		                                       m_pFormatCtx(0),
                                               m_pCodecCtx(0),
                                               m_pCodec(0),
                                               m_open(false) {

	// This registers all available file formats and codecs with the library so
    // they will be used automatically when a file with the corresponding format/codec
    // is opened. Note that you only need to call av_register_all() once, so it's
    // probably best to do this somewhere in your startup code. If you like, it's
    // possible to register only certain individual file formats and codecs, but
    // there's usually no reason why you would have to do that.
	av_register_all();
    avdevice_register_all();

    av_init_packet(&m_packet);

	m_frame_buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE;
	m_frame_buffer = new uint8_t[m_frame_buffer_size];

	m_res_frame_buffer_size = m_frame_buffer_size;
	m_res_frame_buffer = new uint8_t[m_res_frame_buffer_size];
}

CStreamDecoder::~CStreamDecoder() {
	if(m_open) close();
	delete [] m_res_frame_buffer;
	delete [] m_frame_buffer;
}


bool CStreamDecoder::prepareOpen() {
    if(m_open) {
        cerr << "Warning: CStreamDecoder::open called while decoder was still open." << endl
             << "Decoder can be opened only once. Call CStreamDecoder::close() before." << endl;
        return false;
    }
    av_init_packet(&m_packet);
    return true;
}


CStreamFmt CStreamDecoder::openAlsa(std::string alsaDevice, int sample_rate, int channels) {
    if(!prepareOpen()) {
        return CStreamFmt();
    }

    AVDictionary *optionsPtr = NULL;

    AVInputFormat *inputFmtPtr = av_find_input_format("alsa");
    if(inputFmtPtr == NULL)
    {
        std::cerr << "cannot find input for 'alsa'! maybe ffmpeg was built without support for alsa." << std::endl;
    }

    if(sample_rate != -1) {
        av_dict_set_int(&optionsPtr, "sample_rate", sample_rate, 0);
    }
    if(channels != -1) {
        av_dict_set_int(&optionsPtr,"channels", channels, 0);
    }

    return open(alsaDevice, inputFmtPtr, optionsPtr);
}


CStreamFmt CStreamDecoder::openPulse(std::string server,
                                     std::string name,
                                     std::string stream_name,
                                     int sample_rate,
                                     int channels,
                                     int frame_size,
                                     int fragment_size)
{
    if(!prepareOpen()) {
        return CStreamFmt();
    }
    return CStreamFmt();
}



CStreamFmt CStreamDecoder::openFile(std::string filename) {
    if(!prepareOpen()) {
        return CStreamFmt();
    }
    return open(filename, NULL, NULL);
}

CStreamFmt CStreamDecoder::openUrl(std::string url, int timeout_in_ms) {
    if(!prepareOpen()) {
        return CStreamFmt();
    }
    avformat_network_init();

    AVDictionary *optionsPtr = NULL;
    if(timeout_in_ms != -1) {
        av_dict_set_int(&optionsPtr, "timeout", timeout_in_ms, 0);
    }

    return open(url, NULL, optionsPtr );
}


CStreamFmt CStreamDecoder::open(string filename, AVInputFormat *inputFmtPtr, AVDictionary *optionsPtr)
{
	m_filename = filename;

    // The last three parameters specify the file format, buffer size and format parameters;
	// by simply specifying NULL or 0 we ask libavformat to auto-detect the format and
	// use a default buffer size.
	if(avformat_open_input(&m_pFormatCtx, filename.c_str(), inputFmtPtr, &optionsPtr) != 0 )
		cout << " Couldn't open file " << filename << endl;

	// Next, we need to retrieve information about the streams contained in the file:
	if(avformat_find_stream_info(m_pFormatCtx, NULL) < 0 )
		cerr << "Couldn't find stream information in file " << filename << endl;

	// dump_format(m_pFormatCtx, 0, filename, false);


	m_audioStreamID = -1;

	for(unsigned i=0; i < m_pFormatCtx->nb_streams; i++) {
		if(m_pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO) {
			m_audioStreamID = i;
			break;
		}
	}
	if(m_audioStreamID == -1) {
		cerr << "Didn't find a audio stream in file " << filename << endl;
		return CStreamFmt();
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

	m_posInSecs = 0;
	if(m_streamSrvPtr != 0) {
		// set pos to 0 to ensure gui client see 0 seconds progress so they can set title, artist, etc of new song in the gui.
		m_streamSrvPtr->reportProgress( m_posInSecs, m_durationInSecs );
	}

	av_dump_format(m_pFormatCtx, 0, filename.c_str(), false);

	m_pCodec=avcodec_find_decoder(m_pCodecCtx->codec_id);
	if(m_pCodec==NULL)
		cerr << "Codec type " <<  m_pCodecCtx->codec_id << " not found." << endl;

	// Open the codec found suitable for this stream in the last step
	if(avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
		cerr << " Could not open codec." << endl;


//	m_pResamplerCtx = avresample_alloc_context();
//	av_opt_set_int(m_pResamplerCtx, "in_channel_layout",  AV_CH_LAYOUT_STEREO,  0);
//	av_opt_set_int(m_pResamplerCtx, "out_channel_layout", AV_CH_LAYOUT_STEREO,  0);
//	av_opt_set_int(m_pResamplerCtx, "in_sample_rate",     44100,                0);
//	av_opt_set_int(m_pResamplerCtx, "out_sample_rate",    44100,                0);
//	av_opt_set_int(m_pResamplerCtx, "in_sample_fmt",      AV_SAMPLE_FMT_S16P,   0);
//	av_opt_set_int(m_pResamplerCtx, "out_sample_fmt",     AV_SAMPLE_FMT_S16,    0);

	m_open = true;
	m_terminate = false;

	CStreamFmt sfmt = getStreamFmt();

	return sfmt;
}

void CStreamDecoder::startDecodingThread() {
	assert(m_open);

	LOG4CPLUS_DEBUG(CApp::logger(), "CStreamDecoder::startDecodingThread ...");
	m_play_thread = new thread(&CStreamDecoder::decodingLoop, this);
}


void CStreamDecoder::close()
{
	m_terminate = true;
	m_play_thread->join();

	// Free old packet
    if(m_packet.data != NULL) {
        av_free_packet(&m_packet);
        m_packet.data = NULL;
    }
    // close the Codec
    if( m_pCodecCtx != 0 && m_pCodecCtx->codec != 0) {
    	avcodec_close(m_pCodecCtx);
    	m_pCodecCtx->codec = 0;
    	m_pCodecCtx=0;
    }

    // Close the video file
    if(m_pFormatCtx != 0 ) {
    	avformat_close_input(&m_pFormatCtx);
    	m_pFormatCtx = 0;
    }
    m_open = false;
    m_filename = string();
}

CStreamFmt CStreamDecoder::getStreamFmt() {
	CStreamFmt sfmt;
	if(m_pCodecCtx != 0) {
		sfmt.setNumChannels(m_pCodecCtx->channels);
		sfmt.setSampleRate(m_pCodecCtx->sample_rate);

		enum AVSampleFormat sf = m_pCodecCtx->sample_fmt;
		sfmt.setSampleFormat(sf);
	}
	return sfmt;
}


int CStreamDecoder::decode() {
	// prepare a buffer to store the decoded samples in
	// AVFrame *av_frame = avcodec_alloc_frame();
	AVFrame *av_frame = av_frame_alloc();

	bool end_of_stream = false;

    // read and forget packets until a packet with the
	// right stream ID (audioStreamID defined above) is found.
	do {
		// Free old packet
		if (m_packet.data != NULL) {
			av_free_packet(&m_packet);
			m_packet.data = NULL;
		}

		// Read new frame
		if (av_read_frame(m_pFormatCtx, &m_packet) < 0) {
			end_of_stream = true;
			return -1;
		}
	} while (m_packet.stream_index != m_audioStreamID);
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
	int bytesUsed;
	int got_frame;
	//bytesUsed = avcodec_decode_audio3(m_pCodecCtx, sampleBuffer, &sb, &m_packet);
	bytesUsed = avcodec_decode_audio4(m_pCodecCtx, av_frame, &got_frame, &m_packet);

	if (got_frame) {
		int plane_size;
		/* if a frame has been decoded, output it */
		int data_size = av_samples_get_buffer_size(&plane_size,
				                                   m_pCodecCtx->channels,
				                                   av_frame->nb_samples,
				                                   m_pCodecCtx->sample_fmt,
				                                   0);

//		avresample_convert	(	m_pResamplerCtx,
//								&m_res_frame_buffer,
//								1,
//								data_size,
//								av_frame->data,
//								plane_size,
//								data_size );
        uint16_t *out = (uint16_t *)m_res_frame_buffer;
        int write_p=0;

		switch (m_pCodecCtx->sample_fmt){

			case AV_SAMPLE_FMT_S16P:
				for (int nb=0;nb<plane_size/sizeof(uint16_t);nb++){
					for (int ch = 0; ch < m_pCodecCtx->channels; ch++) {
						out[write_p] = ((uint16_t *) av_frame->extended_data[ch])[nb];
						write_p++;
					}
				}
				m_streamSrvPtr->write((char*) out, data_size);
				break;

			case AV_SAMPLE_FMT_S16:
				m_streamSrvPtr->write((char*)av_frame->data[0], data_size);
				break;

			default:
				break;
		}


		// only call m_streamSrvPtr->reportProgress every second
		int64_t tmp = m_packet.pts * m_timeBase.num;
		int playedInSecs = tmp / m_timeBase.den;
		if (playedInSecs > m_posInSecs) {
			m_posInSecs = playedInSecs;
			m_streamSrvPtr->reportProgress(m_posInSecs, m_durationInSecs);
		}
	}

	return bytesUsed;
}


void CStreamDecoder::decodingLoop() {
	while( m_terminate == false ) {
		decode();
	}
}


