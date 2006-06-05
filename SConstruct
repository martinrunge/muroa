#!/bin/env python

import os
import os.path
import string


import SCons.Util

prefixable_constriction_variables = Split('AR AS CC CXX RANLIB')  # LINK
availabe_cross_tools = Split('addr2line c++ g++ gcc-3.4.4 objcopy readelf strip ar c++filt g77 ld objdump size as cpp gcc nm ranlib strings')

def compiler_arm_prefix(env):
  #print(env._dict.keys())
  
  root_dir_tools = '/home/martin/openembedded/build/tmp/'
  staging_postfix = '/staging/arm-linux/'
  cross_tools_postfix = '/cross/bin/'
  
  cross_toos_dir = root_dir_tools + cross_tools_postfix
  staging_dir = root_dir_tools + staging_postfix
    
  path = env['ENV'].get('PATH', [])
  if not path:
    path = []
  if SCons.Util.is_String(path):
    path = string.split(path, os.pathsep)
  
  env['ENV']['PATH'] = string.join([cross_toos_dir] + path, os.pathsep)
  
  cross = 'arm-linux-'
  env['AR'] = cross + 'ar'
  env['AS'] = cross + 'as'
  env['CC'] = cross + 'gcc'
  env['CXX'] = cross + 'g++'
  env['RANLIB'] = cross + 'ranlib'
  #env['LINK'] = cross + 'ld'
  #env['AR'] = cross + 'ar'
  
  env.Append(CPPPATH= staging_dir + 'include')
  env.Append(LIBPATH= staging_dir + 'lib')
  
  #  for var in prefixable_constriction_variables:
  #    print(var , '=' ,  env[var])
  #    tmpstring = cross_prefix + '-' + env[var]
  #    print(tmpstring)
  #    env[var] = tmpstring
  
  return env
    
 
      
# arm-linux-addr2line  
# arm-linux-c++        arm-linux-g++      arm-linux-gcc-3.4.4      arm-linux-objcopy    arm-linux-readelf  arm-linux-strip
# arm-linux-ar         arm-linux-c++filt  arm-linux-g77            arm-linux-ld         arm-linux-objdump  arm-linux-size
# arm-linux-as         arm-linux-cpp      arm-linux-gcc            arm-linux-nm         arm-linux-ranlib   arm-linux-strings





env = Environment()
env.Append(CPPPATH=Split("#libdsaudio #libdsserver #libsock++"))
env.Append(LIBPATH=Split("#libdsaudio #libdsserver #libsock++"))

prefix = '/usr/'
           
                       
                
env = compiler_arm_prefix(env)
                         
                         
Export('env')

SConscript('libdsaudio/SConscript', duplicate=0) 
SConscript('libsock++/SConscript', duplicate=0) 
SConscript('libdsserver/SConscript', duplicate=0) 
SConscript('dsclient/SConscript', duplicate=0) 
SConscript('dsserver/SConscript', duplicate=0) 


        
    