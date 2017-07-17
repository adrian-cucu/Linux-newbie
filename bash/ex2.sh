#!/bin/bash

if [ $# -lt 1 ]; 
  then
	echo "Usage: '${0##*/}' <filename> ..."
	exit 1
fi 

if [ -e "$1" ]; 
  then
	echo "'$1' in current directory..."

	if [ -f "$1" ];
	  then
		echo "'$1' is a file"
	
	else
		echo "'$1' is NOT a file"
	
	fi

else
	echo "'$1' NOT in current directory..."

fi
