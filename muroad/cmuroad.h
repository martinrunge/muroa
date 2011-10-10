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
#ifndef CMUROAD_H
#define CMUROAD_H

/**
main class for muroa deamon. Takes care about command line arguments and config file.

	@author Martin Runge <martin.runge@web.de>
*/

#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

class Cmuroad{
public:
    Cmuroad(int argc, char** argv);

    ~Cmuroad();

    void realtimePrio ( bool theValue )
    {
      m_realtime_prio = theValue;
    }
    

    bool realtimePrio() const
    {
      return m_realtime_prio;
    }

    void resamplerQuality ( int theValue )
    {
      m_resampler_quality = theValue;
    }
    

    int resamplerQuality() const
    {
      return m_resampler_quality;
    }

    void privateOffsetInus ( int theValue )
    {
      m_private_offset_in_us = theValue;
    }
    

    int privateOffsetInus() const
    {
      return m_private_offset_in_us;
    }

    void audioDevice ( const std::string& theValue )
    {
      m_audio_device = theValue;
    }
    

    std::string audioDevice() const
    {
      return m_audio_device;
    }

    void logLevel ( int theValue )
    {
      m_log_level = theValue;
    }
    

    int logLevel() const
    {
      return m_log_level;
    }

    std::string runAsUser() const
    {
      return m_run_as_user;
    }

    std::string runInDir() const
    {
      return m_run_in_dir;
    }
    void daemonize();

    int port() const
    {
        return m_port;
    }

    int maxIdle() const
    {
        return m_max_idle;
    }
    
    
    
    
private:
    po::options_description m_descr;
    po::variables_map m_var_map;

/**
\brief variables hold runtime changeable configuration details 

These variables hold information that may be changed during runtime by d-bus notifications. Here is the central place to store them. Use get and set methods to access or change them.

*/

    std::string m_audio_device;
    const std::string m_audio_device_config_string;
        
    bool m_realtime_prio; 
    const std::string m_realtime_prio_config_string;
    
    int m_resampler_quality;
    const std::string m_resampler_quality_config_string;
    
    int m_private_offset_in_us;
    const std::string m_private_offset_in_us_config_string;

    bool m_fork;
    const std::string m_fork_config_string;

    int m_log_level;
    const std::string m_log_level_config_string;
    
    std::string m_run_as_user;
    const std::string m_run_as_user_config_string;

    std::string m_run_in_dir;
    const std::string m_run_in_dir_config_string;
    
    int m_port;
    const std::string m_port_config_string;
    
    int m_max_idle;
    const std::string m_max_idle_config_string;

    
private:
    void parseOptions(int argc, char** argv);

    void addDescription();
    void forkDaemon();
    void setScheduler();
    void switchUser();

};

#endif
