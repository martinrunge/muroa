#/bin/bash

# set -x

echo "deploy build artifacts into debian package template..."
echo "call from top level build directory"


PROJDIR=$(dirname $0)/../../
DEBDIR=$PROJDIR/aux/packaging/deb/muroa

REPODIR=$HOME/raspirepo/

echo "DEBDIR=$(realpath $DEBDIR)"

pushd $DEBDIR/..

while read debfile
do
    echo "Found old package file '$debfile'. Removing it .."
    if [ -f $debfile ]; then
      rm -f $debfile
    fi
done < <(find \( -name "*.deb" -or -name "*.changes" \) -type f)


popd
make install DESTDIR=$DEBDIR


# do the actual package building on a raspberry pi
ssh pi@raspi-deb-packer "pwd && git/muroa/aux/scripts/build-deb-pkg.sh"


#
# call thos on targets to add signing key:
# sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 5FBF5DA1
#
pushd $DEBDIR/..

dpkg_list=()
while IFS= read -d $'\0' -r file ; do
    dpkg_list=("${dpkg_list[@]}" "$file")
done < <(find -name "*.deb" -print0)

# echo "DIE Liste:   ${dpkg_list[@]}"

for file in "${dpkg_list[@]}" ; do
    echo "package was built: '$file' -> adding it to repository in '$REPODIR'"
    reprepro -b $REPODIR includedeb jessie $file
done

popd

#FN=$(find $DEBDIR/.. -name "*.deb" -type f)
#if [ $? -ne 0 ]; then
#    echo "Error: no debian package was found inside $DEBDIR/.."
#    echo "Maybe the build on the RaspberryPi was no successful or the directory is not mounted the expected way"
#else
#    echo "package was built: '$FN' -> adding it to repository in '$REPODIR'"
#    reprepro -b $REPODIR includedeb jessie $FN
#fi


# mirror the repo to deb.muroa.org
cd $DEBDIR
rsync -avze ssh .  u71829316@home449687942.1and1-data.host:~/deb/raspbian/

