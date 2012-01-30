#!/bin/bash

FILENAME=$1
BASEDIR=$(dirname $FILENAME)
CMDFILE=$(mktemp)

cat <<EOF > $CMDFILE
file ${FILENAME}
save gdb-index ${BASEDIR}
quit
EOF

gdb -x $CMDFILE

[ -f $FILENAME.gdb-index ] || echo "Error, gdb index file not created"

objcopy --add-section .gdb_index=$FILENAME.gdb-index --set-section-flags .gdb_index=readonly $FILENAME $FILENAME

rm $CMDFILE

