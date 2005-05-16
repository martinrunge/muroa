/***************************************************************************
 *   Copyright (C) 2005 by Martin Runge   *
 *   martin.runge@web.de   *
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
#ifndef CSYNC_H
#define CSYNC_H

/**
synchronisation objects which transport a timestamp at which a sample in a specified stream should be played.

@author Martin Runge
*/

#include <boost/date_time/posix_time/posix_time.hpp>
#include <linux/types.h>

class CRTPPacket;


typedef struct serialisation_vars{
  /* YYYYMMDDTHHMMSS,fffffffff */
  char timestamp[32]; // only 27 used
  uint32_t sample_nr;
  uint32_t stream_id;
  uint32_t session_id;
}serialisation_vars_t;

typedef union m_serialization_buffer_t{
  serialisation_vars_t serialisation_vars;
  char raw_buffer[32 + 4 + 4 + 4];
};


class CSync{
public:
    /** default contructor: takes the actual system time */
    CSync();

    /** give a point of time */
    CSync(boost::posix_time::ptime time);

    /** construct a sync object roght from the RTP packet that transported it */
    CSync(CRTPPacket* rtp_packet);

    ~CSync();

    inline uint32_t sampleNr(void) { return m_sample_nr; };
    inline void sampleNr(uint32_t sample_nr) { m_sample_nr = sample_nr; };

    inline uint32_t streamId(void) { return m_stream_id; };
    inline void streamId(uint32_t stream_id) { m_stream_id = stream_id; };

    inline uint32_t sessionId(void) { return m_session_id; };
    inline void sessionId(uint32_t session_id) { m_session_id = session_id; };

    char* serialize();
    void deserialize();
    int getSerialisationBufferSize();
    char* getSerialisationBufferPtr();
    void print();
    void addTimePeriod(boost::posix_time::time_duration duration);
    void addDuration(boost::posix_time::time_duration duration);

    inline boost::posix_time::ptime* getPtimePtr() { return m_local_time; };


private:
  /** point of time at which the specified sample number should be played back */
  boost::posix_time::ptime *m_local_time;
  
  /** the sample number that belongs to the specified point of time */
  uint32_t m_sample_nr;

  /** session id */
  uint32_t m_session_id;

  /** stream id */
  uint32_t m_stream_id;

  union m_serialization_buffer_t m_serialization_buffer;


protected:

};

#endif
