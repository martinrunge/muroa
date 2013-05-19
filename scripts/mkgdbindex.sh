#!/bin/bash

FILENAME=$1
BASEDIR=$(dirname $FILENAME)
CMDFILE=$(mktemp)

echo""
echo "inserting gdbindex into $FILENAME (pwd: $(pwd)  BASEDIR: $BASEDIR)"

cat <<EOF > $CMDFILE
file ${FILENAME}
save gdb-index ${BASEDIR}
quit
EOF

gdb -x $CMDFILE

if [ -f $FILENAME.gdb-index ]; then
  objcopy --add-section .gdb_index=$FILENAME.gdb-index --set-section-flags .gdb_index=readonly $FILENAME $FILENAME
else
  echo "No gdb index created"
fi

rm $CMDFILE
echo""

