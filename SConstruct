#!/bin/env python

import os
import os.path
import string


import SCons.Util



def compiler_arm_prefix(env):
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

	env.Append(CPPPATH= staging_dir + 'include')
	env.Append(LIBPATH= staging_dir + 'lib')

	return env


opts = Options()
opts.Add('target', 'select target for cross compiling (x86, arm, ...)', 'x86')



x86_env = Environment(optiosn = opts)
x86_env.Append(CPPPATH=Split("#libdsaudio #libdsserver #libsock++"))

arm_env = x86_env.Copy()
arm_env.Append(LIBPATH=Split("#arm-obj/libdsaudio #arm-obj/libdsserver #arm-obj/libsock++"))
x86_env.Append(LIBPATH=Split("#x86-obj/libdsaudio #x86-obj/libdsserver #x86-obj/libsock++"))

prefix = '/usr/'


print(opts.args)
try:
  target = opts.args['target']
  
except:
  print("target not set. Using x86")  
  target = 'x86'

print('target=', target)


if 'arm' in target:
	print('Compiling for arm')
	arm_env = compiler_arm_prefix(arm_env)
	build_dir = 'arm-obj'
	env = arm_env
if 'x86' in target:
	print('Compiling for x86')
	build_dir = 'x86-obj'
	env = x86_env

Help(opts.GenerateHelpText(env))



Export('env')


subdirs = Split('libdsaudio libsock++ libdsserver dsclient dsserver')

for subdir in subdirs:
	env.BuildDir(build_dir + '/' + subdir, subdir)
	env.SConscript(subdir + '/SConscript', build_dir = build_dir + '/' + subdir, duplicate=0) 

