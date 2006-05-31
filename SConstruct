#!/bin/env python


env = Environment()
env.Append(CPPPATH=Split("#libdsaudio #libdsserver #libsock++"))
env.Append(LIBPATH=Split("#libdsaudio #libdsserver #libsock++"))

Export('env')

SConscript('libdsaudio/SConscript', duplicate=0) 
SConscript('libsock++/SConscript', duplicate=0) 
SConscript('libdsserver/SConscript', duplicate=0) 
SConscript('dsclient/SConscript', duplicate=0) 
SConscript('dsserver/SConscript', duplicate=0) 

