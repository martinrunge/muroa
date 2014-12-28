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
#ifndef IAUDIOIO_H
#define IAUDIOIO_H

#include <string>

enum sound_system {
  SOUND_SYS_OSS,
  SOUND_SYS_ALSA
};

/**
@author Martin Runge
*/
class IAudioIO{
public:
	enum state_t {
		E_OPEN,
		E_SETUP,
		E_PREPARED,
		E_RUNNING,
		E_UNDERRUN,
		E_PAUSED,
		E_CLOSED,
		E_NONE
	};

	IAudioIO();

    virtual ~IAudioIO();


    virtual int close() = 0;
    virtual int open(std::string device, int samplerate, int channels) = 0;
    virtual int read(char* data, int buffersize) = 0;
    virtual int write(char* data, int length) = 0;
    virtual int start() = 0;
    virtual int stop() = 0;
    
    virtual int state() = 0;

    virtual int getOutOverflows(void) = 0;
    virtual int getBytesPlayed(void) = 0;
    virtual int getDelay() = 0;
    virtual int getSpace() = 0;

    virtual int getWriteGranularity(void) = 0;
    virtual int getActualSampleRate(void) = 0;

    bool checkSoundSystem(enum sound_system system);
    
protected: 
	 bool m_is_open;
};

#endif
