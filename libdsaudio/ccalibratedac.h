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
#ifndef CCALIBRATEDAC_H
#define CCALIBRATEDAC_H

/**
Class imlements an initial measurement of the actual sampling rate of the sound card.

@author Martin Runge
*/

#include <sys/time.h>

class CAudioOSS;

class CCalibrateDAC{
public:
    CCalibrateDAC();

    ~CCalibrateDAC();
    
    double measureSampleRate();

private:
    void doCalibrate();
  
    int getNumOfCycles();
    
    int m_test_time_in_s;
    int m_num_cycles;
    
    int m_num_test_samples;
    int m_num_channels;
    int m_sample_rate;
    int m_sample_size;

    int m_bytes_per_sec;
        
    short *m_testbuffer;
    int m_testbuffer_size;
    CAudioOSS* m_audio;

    double m_measured_samping_rate;
};

#endif
