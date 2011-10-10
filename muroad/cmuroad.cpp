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
#include "cmuroad.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h> 

#include <boost/program_options/parsers.hpp>

using namespace std;

Cmuroad::Cmuroad(int argc, char** argv): 
    m_descr("Allowed options"),
    m_audio_device_config_string("device"),
    m_realtime_prio_config_string("realtime-prio"),
    m_resampler_quality_config_string("resampler-quality"),
    m_private_offset_in_us_config_string("offset"),
    m_fork_config_string("fork"),
    m_log_level_config_string("loglevel"),                         
    m_run_as_user_config_string("user"),
    m_run_in_dir_config_string("run-dir"),
    m_port_config_string("port"),
    m_max_idle_config_string("max-idle")
                           
{

  addDescription();
  parseOptions(argc, argv);
  
}


Cmuroad::~Cmuroad()
{
}




/*!
    \fn Cmuroad::addDescription
 */
void Cmuroad::addDescription()
{
  // Declare the supported options.
  m_descr.add_options()
    ("help", "produce help message")
    (m_audio_device_config_string.c_str(), po::value<string>(&m_audio_device)->default_value("hw:0,0"), "specify audio device (soundcard) for output")
    (m_realtime_prio_config_string.c_str(), "try to get realtime priority for playback. Drop any unneeded privileges after setting them.")
    (m_resampler_quality_config_string.c_str(), po::value<int>(&m_resampler_quality)->default_value(2), "quality setting for the resampling filter: 0 simple, 1 middle, 2 high quality.")
    (m_private_offset_in_us_config_string.c_str(), po::value<int>(&m_private_offset_in_us)->default_value(0), "Offset for audio playback in micro seconds.")
      (m_fork_config_string.c_str(), "fork to background and run in daemon mode.")
    (m_log_level_config_string.c_str(), po::value<int>(&m_log_level)->default_value(0), "Log level: 0 errors only, 1 warnings, 2 info, 3-10 debug.")
    (m_run_as_user_config_string.c_str(), po::value<string>(&m_run_as_user)->default_value("muroa"), "run daemon as special user. Useful, becaue you don't need a shell account for muroa then.")
    (m_run_in_dir_config_string.c_str(), po::value<string>(&m_run_in_dir)->default_value("/var/run"), "directory in which the murad should run. core files will appear there :-)")
    (m_port_config_string.c_str(), po::value<int>(&m_port)->default_value(4001), "port number to listen for RTP packets on.")
    (m_max_idle_config_string.c_str(), po::value<int>(&m_max_idle)->default_value(60), "time in seconds to wait before closing the audio device when no more data is received. Audio device will be opened again as soon as further audio packets are received. Specify 0 here to keep audio device always open.")
    ;
  

}


/*!
    \fn Cmuroad::praseOptions()
 */
void Cmuroad::parseOptions(int argc, char** argv)
{
  ifstream ifs("muroa.conf");
  
  po::store(po::parse_command_line(argc, argv, m_descr), m_var_map);
  po::store(po::parse_config_file(ifs, m_descr), m_var_map);
  po::notify(m_var_map);
  
  if (m_var_map.count("help")) {
    cout << m_descr << endl;
    throw string("help");
  }

  // see, if audio device was specified
  if (m_var_map.count(m_audio_device_config_string)) {
    cout << "audio device " << m_audio_device << " is selected." << endl;
  } else {
    cout << "audio device was not specified on commandline." << endl;
  }
  
  // see, if specified wether to try to use realtime priority
  if (m_var_map.count(m_realtime_prio_config_string)) {
    cout << "try to get realtime prio for playback." << endl;
    m_realtime_prio = true;
  }
  else 
  {
     cout << "not trying to get realtime prio for playback." << endl;
     m_realtime_prio = false;
  }
  
  // see what resampler quality to use
  if (m_var_map.count(m_resampler_quality_config_string)) {
    cout << "resampler quality " << m_resampler_quality << " is selected." << endl;
  } else {
    cout << "resampler quality was not specified on commandline or config file." << endl;
  }

  // see, if to use private offset in this client
  if (m_var_map.count(m_private_offset_in_us_config_string)) {
    // m_private_offset_in_us = m_var_map[m_private_offset_in_us_config_string].as<int>();
    cout << "private offset specified to " << m_private_offset_in_us << " mico seconds." << endl;
  } else {
    cout << "private offset was not specified on commandline or config file." << endl;
  }


  // see, if to use private offset in this client
  if (m_var_map.count(m_fork_config_string)) {
    // m_private_offset_in_us = m_var_map[m_private_offset_in_us_config_string].as<int>();
    cout << "going to fork into background." << endl;
    m_fork = true;
  } else {
    cout << "staying on console. Do not fork into background" << endl;
    m_fork = false;
  }

  // see what resampler quality to use
  if (m_var_map.count(m_log_level_config_string)) {
    cout << "Loglevel: " << m_log_level << " is selected." << endl;
  } else {
    cout << "Loglevel missing! There should be a default value!" << endl;
  }
  
  // see, if audio device was specified
  if (m_var_map.count(m_run_as_user_config_string)) {
    cout << "going to run as user user " << m_run_as_user << " ." << endl;
  } else {
    cout << "run as user missing! There should be a default value!" << endl;
  }


  // see, if audio device was specified
  if (m_var_map.count(m_run_in_dir_config_string)) {
    cout << "going to run in " << m_run_in_dir << " ." << endl;
  } else {
    cout << "run dir is missing! There should be a default value!" << endl;
  }
  
  // see what resampler quality to use
  if (m_var_map.count(m_port_config_string)) {
    cout << "using port number: " << m_port  << endl;
  } else {
    cout << "port number missing! There should be a default value!" << endl;
  }
  
    // see what resampler quality to use
  if (m_var_map.count(m_max_idle_config_string)) {
    cout << "Idle time before closing sound device: " << m_max_idle << " seconds." << endl;
  } else {
    cout << "Max idle time missing! There should be a default value!" << endl;
  }

}


/*!
    \fn Cmuroad::setScheduler()
 */
void Cmuroad::setScheduler()
{
  struct sched_param s_param;

  sched_getparam(0, &s_param);
  s_param.sched_priority = 50;
  sched_setparam(0, &s_param);
  int res = sched_setscheduler(0, SCHED_FIFO, &s_param);
  if (res != 0)
  {
    perror("set realtime prio");
    cerr << "continueing without realtime priority!" << endl;
    m_realtime_prio = false;
  }
}


/*!
    \fn Cmuroad::fork()
 */
void Cmuroad::forkDaemon()
{
  cerr << " forking to background ..." << endl;
  pid_t fork_pid=fork();
  if (fork_pid < 0)
  {
    perror("fork");
    exit(1); /* fork error */
  }

  if(fork_pid == 0) {
    pid_t pid = setsid(); /* obtain a new process group */
    if(pid == -1)
    {
      perror("setsid");
    }
  }    
  if (fork_pid > 0)
  {
    exit(0); /* parent exits */
  }
  /* child (daemon) continues */

  cerr << "changing run dir to " << m_run_in_dir << endl;
  int res = chdir(m_run_in_dir.c_str());
  if(res != 0)
  {
    perror("chdir");
    cerr << "trying cd to / ..." << endl;
    res = chdir("/");
    if(res != 0)
    {
      perror("chdir");
      cerr << "giving up!" << endl;
      exit(1);
    }
  }

  //redirect stdin, stdout and stderr to /dev/null
  int fd = open("/dev/null",O_RDWR); /* open stdin */
  dup(fd); /* stdout */
  dup(fd); /* stderr */

}


/*!
    \fn Cmuroad::switchUser()
 */
void Cmuroad::switchUser()
{
  struct passwd* pwd;
  int retval;
  
  pwd = getpwnam(m_run_as_user.c_str());
  if(pwd != 0)
  {
    cerr << "setting user to " << m_run_as_user << " (UID=" << pwd->pw_uid << ")...";
    setgid(pwd->pw_gid);
    retval = setuid(pwd->pw_uid);
    if(retval == 0)
    {
      
      cerr << "success." << endl;
    }
    else
    {
      cerr << "setting UID faild (" << strerror(errno) << "), trying to set EUID insted...";
      setegid(pwd->pw_gid);
      retval = seteuid(pwd->pw_uid);
      if(retval == 0)
      {
        
        cerr << " success." << endl;
      }
      else
      {
        cerr << "setting EUID faild (" << strerror(errno) << "). Continuing as started" << endl;;
      }
    }
  }
  else
  {
    cerr << "Cannot switch user to '" << m_run_as_user << "'. User does not seem to exists. Continuing as started." << endl;
  }

  cerr << "gid: " << getgid() << " egid: " << getegid() << endl;
}


/*!
    \fn Cmuroad::daemonize()
 */
void Cmuroad::daemonize()
{
  if(m_realtime_prio)
    setScheduler();
  
  switchUser();

  if(m_fork)
    forkDaemon();

}
