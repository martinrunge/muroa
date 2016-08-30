#/bin/bash

# set -x

echo "deploy build artifacts into debian package template..."
echo "call from top level build directory"


PROJDIR=$(dirname $0)/../../
DEBDIR=$PROJDIR/aux/deb-pkg/muroa

REPODIR=$HOME/raspirepo/

echo "DEBDIR=$(realpath $DEBDIR)"

mkdir -p $DEBDIR/usr/bin
mkdir -p $DEBDIR/usr/lib
mkdir -p $DEBDIR/etc

install -m 755 muroad/muroad  $DEBDIR/usr/bin
install -m 755 libmcommons/libmcommons.so  $DEBDIR/usr/lib
install -m 755 libmstream/libmstream.so  $DEBDIR/usr/lib
install -m 755 libmuroadaemon/libmuroadaemon.so  $DEBDIR/usr/lib
install -m 755 libsock++/libsock++.so  $DEBDIR/usr/lib


CONTROLFILE=$DEBDIR/DEBIAN/control

VERSIONSTR="0.6-$(date +%Y%m%dT%H%M%S)"

sed -i "s/Version:.*/Version: $VERSIONSTR/g"  $CONTROLFILE


cd $DEBDIR/..

dpkg-deb -b muroa muroad_$VERSIONSTR.deb

#
# call thos on targets to add signing key:
# sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 5FBF5DA1
#

reprepro -b $REPODIR includedeb jessie muroad_$VERSIONSTR.deb
