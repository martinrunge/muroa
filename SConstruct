#!/bin/env python


prefixable_constriction_variables = Split('AR AS CC CXX LINK RANLIB SHCC SHCXX SHLINK ')
availabe_cross_tools = Split('addr2line c++ g++ gcc-3.4.4 objcopy readelf strip ar c++filt g77 ld objdump size as cpp gcc nm ranlib strings')

def compiler_prefix(env, cross_prefix):
  # print(dir(env))
  
  for var in env._dict:
    if(available_cross_tools evn[var]):
      print(var , " = ", env[var]) 
  return env
    
 
      
# arm-linux-addr2line  
# arm-linux-c++        arm-linux-g++      arm-linux-gcc-3.4.4      arm-linux-objcopy    arm-linux-readelf  arm-linux-strip
# arm-linux-ar         arm-linux-c++filt  arm-linux-g77            arm-linux-ld         arm-linux-objdump  arm-linux-size
# arm-linux-as         arm-linux-cpp      arm-linux-gcc            arm-linux-nm         arm-linux-ranlib   arm-linux-strings





env = Environment()
env.Append(CPPPATH=Split("#libdsaudio #libdsserver #libsock++"))
env.Append(LIBPATH=Split("#libdsaudio #libdsserver #libsock++"))

prefix = '/usr/'
           
cross = 'arm-linux'                       
                
env = compiler_prefix(env, cross)
                         
                         
Export('env')

SConscript('libdsaudio/SConscript', duplicate=0) 
SConscript('libsock++/SConscript', duplicate=0) 
SConscript('libdsserver/SConscript', duplicate=0) 
SConscript('dsclient/SConscript', duplicate=0) 
SConscript('dsserver/SConscript', duplicate=0) 


        
    