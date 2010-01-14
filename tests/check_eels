#!/bin/sh

echo -n "run 'eels' = "
X=$(./eels)
echo -n "'$X' "

if [ "$X" = "My hovercraft is full of eels." ]; then
  echo "[ok]"
  exit 0
else
  echo "unexpected output"
fi

exit 1
