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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include "cplayer.h"

using namespace std;

int main(int argc, char *argv[])
{
  int num;
  char c;
  
  string device_str, device;  
  device = "hw:0,0";  

  struct sched_param s_param;
  
  sched_getparam(0, &s_param);
  s_param.sched_priority = 50;
  sched_setparam(0, &s_param);
  sched_setscheduler(0, SCHED_FIFO, &s_param); 

  if(argc == 3) {
    device_str = argv[1];
    if(device_str.compare("--device") == 0)
      device = argv[2];
    
  }

  cout << "dsclient opening device " << device << endl;
  CPlayer player(4001, device);
  player.start();

  cin >> c;

  return EXIT_SUCCESS;
}
