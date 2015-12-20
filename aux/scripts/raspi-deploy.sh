#!/bin/bash

CLIENTS=("$@")
USER="pi"

LIBS=("libdsaudio/libdsaudio.so" \
      "libsock++/libsock++.so" \
      "libmuroadaemon/libmuroadaemon.so")
DAEMON="muroad/muroad"
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
