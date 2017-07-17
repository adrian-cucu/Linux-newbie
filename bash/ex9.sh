#!/bin/bash

echo -n "(#)Normal order : "
for argument; do
	echo -n "$argument "
done
echo

############################## V1
echo -n "(#)Reverse order (V1): "
REVERSE=""
for argument; do
	REVERSE="$argument $REVERSE"
done
echo $REVERSE
############################## V2
echo -n "(#)Reverse order (V2): "
args=("$@")

for ((i=$# - 1; i >= 0; -- i)); do
	echo -n "${args[$i]} "
done
echo
