#!/usr/bin/env python
# -*- coding: utf-8 -*-


import os
from optparse import OptionParser



parser = OptionParser()
parser.add_option("-s", "--sysroot", dest="sysroot", default=".", help="Start at and sysroot to search for absolute links.")

(options, args) = parser.parse_args()


print("Using sysroot: %s"%options.sysroot)
sysroot = os.path.expanduser(options.sysroot)
sysroot = os.path.expandvars(sysroot)
print("Using sysroot expanded: %s"%sysroot)

for root, dirs, files in os.walk(sysroot, topdown=True):
    #print("root: %s"%root)
    for name in files:
        fn = os.path.join(root, name)
        if os.path.islink(fn):
            linktarget = os.readlink(fn)
            # print("symlink: %s -> %s"%( fn, linktarget))
            if os.path.isabs(linktarget):
                pos = linktarget.find(sysroot)
                reltarget = ""
                if pos == 0:
                    # linktarget starts with sysroot, just make it relative
                    reltarget = os.path.relpath(linktarget, os.path.dirname(fn))
                else:
                    linktarget = os.path.join(sysroot, linktarget[1:])
                    reltarget = os.path.relpath(linktarget, os.path.dirname(fn))
                    
                print("symlink: %s -> %s   relative link: %s"%( fn, linktarget, reltarget))
                os.remove(fn)
                os.symlink(reltarget, fn)
                
    #for name in dirs:
    #    print("dir: %s"%name)



print("finished")
