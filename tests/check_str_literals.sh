#!/bin/sh

echo -n "run 'str_literals' = "
X=$(./str_literals)
echo -n "'$X' "

if [ "$X" = "I will not buy this record; it is scratched." ]; then
  echo "[ok]"
  exit 0
else
  echo "unexpected output"
fi

exit 1
