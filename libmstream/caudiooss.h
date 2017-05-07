//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Martin Runge <martin.runge@web.de>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CAUDIOOSS_H
#define CAUDIOOSS_H

#include "iaudioio.h"

/**
Audio output via Open Sound System API

@author Martin Runge
*/
class CAudioOSS : public IAudioIO{
public:
    CAudioOSS();

    ~CAudioOSS();

    int start();
    int stop();
    
    int state();


    int close();
    int closeMixer();
    int open(std::string device, int samplerate, int channels);
    int openMixer(std::string device, std::string channel);

    int setVolume(int volume);
    int read(char* data, int buffersize);
    int write(char* data, int length);

    int getBytesPlayed();
    int getOutOverflows();
    int getDelay();
    int getSpace();
    int getWriteGranularity(void);
    int getActualSampleRate(void);

        
private: 
    int m_snd_dev;
    int m_snd_fmt;
    int m_snd_num_ch;
    int m_snd_rate;
    int m_sample_rate;

};

#endif
