/***************************************************************************
 *   Copyright (C) 2004 by Martin Runge                                    *
 *   martin.runge@web.de                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CAUDIOIOALSA_H
#define CAUDIOIOALSA_H



#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

#include "IAudioIO.h"

/**
@author Martin Runge
*/


class CAudioIoAlsa : public IAudioIO
{
public:
    CAudioIoAlsa();

    ~CAudioIoAlsa();

    int close();
    int open(std::string device, int samplerate, int channels);
    int read(char* data, int buffersize);
    int write(char* data, int length);

    int getBytesPlayed();
    int getOutOverflows();
    int getDelay();
    int getWriteGranularity(void);
    int getActualSampleRate(void);


private:
    
    snd_pcm_t *m_playback_handle;
    snd_pcm_hw_params_t *m_hw_params;
    snd_pcm_sw_params_t *m_sw_params;
    snd_pcm_status_t *m_status_ptr; 

    int m_write_granularity;     
    int m_sample_rate;
    bool m_open;
  
        
};

#endif
