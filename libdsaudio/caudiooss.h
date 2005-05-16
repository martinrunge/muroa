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
    
    int close();
    int open(std::string device, int samplerate, int channels);
    int read(char* data, int buffersize);
    int write(char* data, int length);

    int getBytesPlayed();
    int getOutOverflows();
    int getDelay();
    int getWriteGranularity(void);

        
private: 
    int m_snd_dev;
    int m_snd_fmt;
    int m_snd_num_ch;
    int m_snd_rate;

};

#endif
