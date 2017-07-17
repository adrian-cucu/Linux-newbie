#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Usage '${0##*/}' <fname> ..."
	exit 1
fi

for i; do
	if [ "$(find /tmp -name "$i" 2>/dev/null)" ]; then
		printf "'$i' \33[32mfound\33[00m  in /tmp\n"

	else
		printf "'$i' \33[31mNOT found\33[00m  in /tmp\n"

	fi
done
