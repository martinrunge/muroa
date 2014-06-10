#!/bin/bash

CLIENTS=("$@")
USER="pi"

LIBS=("libdsaudio/build_arm/libdsaudio.so" \
      "libsock++/build_arm/libsock++.so" \
      "libmuroadaemon/build_arm/libmuroadaemon.so")
DAEMON="muroad/build_arm/muroad"
CFG="muroad/muroa.conf"

for RENDERCLIENT in "${CLIENTS[@]}"; do
#

  for f in "${LIBS[@]}"
  do
    scp $f $USER@$RENDERCLIENT:~
  done

  scp $DAEMON $USER@$RENDERCLIENT:~
  scp $CFG $USER@$RENDERCLIENT:~
  
done
