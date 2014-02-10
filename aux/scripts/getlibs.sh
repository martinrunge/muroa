#!/bin/bash

SCRIPTDIR=$(dirname $0)

RASPI=raspi2
X_TOOLS=${HOME}/x-tools
ARCH_TUPLE=arm-unknown-linux-gnueabi
SYSROOT=$X_TOOLS/$ARCH_TUPLE/$ARCH_TUPLE/sysroot
# for testing :
# SYSROOT=${HOME}/tmp/xtoolstest/$ARCH_TUPLE/$ARCH_TUPLE/sysroot

mkdir -p $SYSROOT/usr/


rsync -avz $RASPI:/usr/include  $SYSROOT/usr/
rsync -avz $RASPI:/usr/lib  $SYSROOT/usr/
rsync -avz $RASPI:/lib  $SYSROOT/


$SCRIPTDIR/adoptsymlinks.py --sysroot=$SYSROOT