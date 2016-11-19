#/bin/bash

# set -x

PROJDIR=$(dirname $0)/../../
DEBDIR=$PROJDIR/aux/packaging/deb/muroad

REPODIR=$HOME/raspirepo/
DEBDIR=$(realpath $DEBDIR)


echo "DEBDIR: $DEBDIR"
cd ${DEBDIR}
pwd
debchange -n "Testing package $(date +%Y%m%dT%H%M%S)"

dpkg-buildpackage -b

# dpkg-deb -b muroa muroad_$VERSIONSTR.deb

#
# call thos on targets to add signing key:
# sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 5FBF5DA1
#

# reprepro -b $REPODIR includedeb jessie muroad_$VERSIONSTR.deb
