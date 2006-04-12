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

enum sync_type_t {
  SYNC_NEW_STREAM,
  SYNC_REQ_STREAM,
  SYNC_FLUSH,
  SYNC_CLOSE
};


typedef struct serialisation_vars{
  uint32_t sync_type;
  uint32_t stream_id;
  uint32_t session_id;

  uint32_t frame_nr;
  /* YYYYMMDDTHHMMSS,fffffffff */
  char timestamp[32]; // only 27 used
}serialisation_vars_t;

typedef union m_serialization_buffer_t{
  serialisation_vars_t serialisation_vars;
  char raw_buffer[4 + 4 + 4 + 4 + 32];
};


class CSync{
public:
    /** default contructor: takes the actual system time */
    CSync(enum sync_type_t sync_type = SYNC_NEW_STREAM);

    /** give a point of time */
    CSync(boost::posix_time::ptime time);

    /** construct a sync object roght from the RTP packet that transported it */
    CSync(CRTPPacket* rtp_packet);

    ~CSync();

    inline enum sync_type_t syncType(void) { return m_sync_type; };
    inline void syncType(enum sync_type_t sync_type) { m_sync_type = sync_type; };


    inline uint32_t streamId(void) { return m_stream_id; };
    inline void streamId(uint32_t stream_id) { m_stream_id = stream_id; };

    inline uint32_t sessionId(void) { return m_session_id; };
    inline void sessionId(uint32_t session_id) { m_session_id = session_id; };

    inline uint32_t frameNr(void) { return m_frame_nr; };
    inline void frameNr(uint32_t frame_nr) { m_frame_nr = frame_nr; };

    char* serialize();
    void deserialize();
    int getSerialisationBufferSize();
    char* getSerialisationBufferPtr();
    void print();
    void addDuration(boost::posix_time::time_duration duration);

    inline boost::posix_time::ptime* getPtimePtr() { return m_local_time; };
    void setTimeToNow();


private:

  /** which type of sync object is it */
  enum sync_type_t m_sync_type;


  /** session id */
  uint32_t m_session_id;

  /** stream id */
  uint32_t m_stream_id;

  /** the sample number that belongs to the specified point of time */
  uint32_t m_frame_nr;

  /** point of time at which the specified sample number should be played back */
  boost::posix_time::ptime *m_local_time;


  union m_serialization_buffer_t m_serialization_buffer;


protected:

};

#endif
