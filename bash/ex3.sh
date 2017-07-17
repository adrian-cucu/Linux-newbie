#!/bin/bash

if [ $# -lt 1 ];
  then
	echo "Usage: '${0##*/}' <user name> ..."
	exit 1
fi

#List of connected users from 'who' command the first column
LIST=("$(who | awk '{print $1}')")

args=("$@")

for ((i = 0; i < $#; ++ i));
do
	USER=${args[$i]}
	CONNECTED=0

	for user in $LIST;  do
		if [ $USER = $user ]; then
			CONNECTED=1
			break
		fi
	done

	if (( $CONNECTED )); then
		echo "User '$USER' is connected..."

	else
		echo "User '$USER' is NOT connected..."

	fi
done
