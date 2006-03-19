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


#include "cstreamserver.h"

CStreamServer streamserver(1, 300);

extern "C" {

int ds_cpp_init(void) {

  std::cerr << "ds_cpp_init" << std::endl;
  return 0;

}

int ds_cpp_open(int audio_bytes_per_second) {
  // streamserver = new CStreamServer( 1, 300 );

  CIPv4Address localaddr("localhost:4001", 4001);
  streamserver.addClient(&localaddr);

  streamserver.open(audio_bytes_per_second); 

  return 0;
}

void ds_cpp_close() {
  streamserver.close();
}


int ds_cpp_write(char* buffer, int length) {
  std::cerr << "write " << buffer << " length " << length << std::endl;
  streamserver.write(buffer, length);
  return 0;
}



void ds_cpp_flush() {
  streamserver.flush();
}

void ds_cpp_pause() {
  // streamserver->pause();
}
}
