#!/bin/sh

STR="You great poof."

echo -n "run 'stditostdo' = "
X=$(echo $STR | ./stditostdo)
echo -n "'$X' "

if [ "$X" = "$STR" ]; then
  echo "[ok]"
  exit 0
else
  echo "unexpected output"
fi

exit 1
