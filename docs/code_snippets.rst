Code Snippets
=============


Decode mp3 audio using ffmpeg libraries
---------------------------------------


.. code-block:: c
    :linenos:

    #include <iostream>
    #include <string>
    extern "C" {
    #include <ffmpeg/avformat.h>
    #include <ffmpeg/avcodec.h>
    }
    #include <stdint.h>
    #include <assert.h>
    
    using namespace std;
    
    void sendPacket(CodecID codec_id, uint8_t* data, int length, 
                    int pts, AVRational timebase);
    
    FILE* outfile;
    
    void usage() {
        printf("usage:\n");
        printf("packetexample inputfile <outputfile>\n");
        printf("outputfile deaults to 'outfile.raw' if not specified.\n");
        exit(0);
    }
    
    
    int main(int argc, char** argv) 
    {
    
        string infile_name, outfile_name;
    
        if(argc < 2) {
            usage();   
        }
    
        infile_name = argv[1];
    
        if(argc < 3) {
            outfile_name = "outfile.raw";    
        }
        else {
            outfile_name = argv[2];    
        }
    
        outfile = fopen(outfile_name.c_str(), "wb");
    
        // This registers all available file formats and codecs with the 
        // library so they will be used automatically when a file with the
        // corresponding format/codec is opened. Note that you only need to 
        // call av_register_all() once, so it's probably best to do this  
        // somewhere in your startup code. If you like, it's possible to 
        // register only certain individual file formats and codecs, but 
        // there's usually no reason why you would have to do that.
        av_register_all();
    
        //Next: open the file 'infile_name': 
        AVFormatContext *pFormatCtx;
    
        // The last three parameters specify the file format, buffer size and 
        // format parameters. By simply specifying NULL or 0 we ask libavformat
        // to auto-detect the format and use a default buffer size.  
        if(av_open_input_file(&pFormatCtx, infile_name.c_str(), NULL, 0, NULL)!=0)
        cout << " Couldn't open file " << infile_name << endl; 
    
        // Next, we need to retrieve information about the streams 
        // contained in the file: 
        if(av_find_stream_info(pFormatCtx)<0)
            cerr << "Couldn't find stream information in file " << infile_name.c_str() << endl;
    
        dump_format(pFormatCtx, 0, infile_name.c_str(), false);
    
    
        // The file may contain more than on stream. Each stream can be a 
        // video stream, an audio stream, a subtitle stream or something else.
        // also see 'enum CodecType' defined in avcodec.h
        // Find the first audio stream:
        int audioStreamID = -1;
    
        for(int i=0; i<pFormatCtx->nb_streams; i++) {
        if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_AUDIO) {
    
                audioStreamID=i;
                break;
        }
        }
        if(audioStreamID==-1) {
        cerr << "Didn't find a audio stream in file " << infile_name << endl;
        return -1;
        }
    
        // Get a pointer to the codec context for the audio stream
        AVCodecContext *pCodecCtx;
        pCodecCtx = pFormatCtx->streams[audioStreamID]->codec;
    
    
        // get the timebase for this stream. The presentation timestamp, 
        // decoding timestamp and packet duration is expressed in timestamp 
        // as unit:
        // e.g. if timebase is 1/90000, a packet with duration 4500 
        // is 4500 * 1/90000 seconds long, that is 0.05 seconds == 20 ms.
        AVRational time_base = pFormatCtx->streams[audioStreamID]->time_base;
    
    
        // Find the decoder for the audio stream
        AVCodec *pCodec;
        pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
        if(pCodec==NULL)
        cerr << "Codec type " <<  pCodecCtx->codec_id << " not found." << endl;
    
        // Open the codec found suitable for this stream in the last step
        if(avcodec_open(pCodecCtx, pCodec)<0)
        cerr << " Could not open codec." << endl;
    
        // struct AVPacket: defined in avformat.h:
        // most important attributes in the strcture:
        //
        // packet.data     :the packets payload
        // packet.length   :length of payload in bytes
        // packet.pts      :presentation timestamp in untis of time_base. see above.
        // packet.duration :packet length in time_base unit
        // packet.dts      :decoding timestamp 
    
        static AVPacket packet;
        bool end_of_stream = false;
        do {
            // read and forget packets until a packet with the 
            // right stream ID (audioStreamID defined above) is found.
            do {
                // Free old packet
                if(packet.data!=NULL)
                    av_free_packet(&packet);
    
                // Read new frame
                if(av_read_frame(pFormatCtx, &packet)<0) {
                    end_of_stream = true;   
                    break;
                }  
            } while(packet.stream_index != audioStreamID);
        // here, a new audio packet from the stream is available
    
            cerr << "packetsize = " << packet.size << endl;
    
            // to simulate a network connection, pass just the packet itself, 
            // the codecID and the presentation time to the receiver. This is 
            // the same information like a RTP connection can transport.
            sendPacket(pCodecCtx->codec_id, (uint8_t*)packet.data, 
                    packet.size, packet.pts, time_base);
    
        } while(!end_of_stream);
    
    
        // clean up
    
        // Free old packet
        if(packet.data!=NULL)
        av_free_packet(&packet);
    
        // close the Codec
        avcodec_close(pCodecCtx);
    
        // Close the video file
        av_close_input_file(pFormatCtx);
    
        fclose(outfile);
    
        return 0;   
    
    
    
    
    // this function simulates a RTP receiver. No information from 
    // above is used. The complete decoding is done with the passed 
    // information. Usually, the codec ID and the timebase are passed 
    // via the Session Description Protocol (SDP) from sender to 
    // receiver. The presentation timestamp (pts) and data length is 
    // included in the header of each RTP packet followed by the data 
    // itself.
    void sendPacket(CodecID codec_id, uint8_t* data, int length, 
                    int pts, AVRational timebase) {
        // use local version of these
        static AVCodec *pCodec = 0;
        static AVCodecContext* pContext = 0;
    
        // prepare a buffer to store the decoded samples in
        const int sample_buf_size = 2 * AVCODEC_MAX_AUDIO_FRAME_SIZE;
        static int16_t sample_buffer[sample_buf_size];    
    
        // Find the decoder for the audio stream. The stream ID has to be provided.
        // In case of an RTP connection, a dynmic payload format exchanged via SDP 
        // is often used to inform the receiver about the codecID.
        if(pCodec == 0) 
        {
            pCodec=avcodec_find_decoder(codec_id);
            if(pCodec==NULL)
            {
                cerr << "Codec type " <<  codec_id << " not found." << endl;
            }
        }
    
        // allocate a new AVCodecContext and fill it when opening the codec.
        if(pContext == 0)
        {
            pContext = avcodec_alloc_context();
            if (avcodec_open(pContext, pCodec) < 0)
            {
                return;
            }
        }
    
        // when calling avcodec_decode_audio2, sb hold the sample buffer size, 
        // on return the number of produced samples is stored here. bytes_used
        // indicates how many bytes of the data was used for decoding. When 
        // provided with a self contained packet, it should be used completely.
        int sb = sample_buf_size;
        int bytes_used;
        bytes_used = avcodec_decode_audio2(pContext, sample_buffer, &sb,
                                        data, length);
    
        // write the decoded data to the output file.
        fwrite(sample_buffer, sb, 1, outfile);
    
        // receiver cleanup code omitted in this function to keep it simple.
        // when finished with receiving, cleanu up similar to main function. 
        // close the Codec like this:
        // avcodec_close(pContext); 
    }
    
.

