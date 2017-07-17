#!/bin/bash

read -p "Enter a number with 2 digits: " IN

case $IN in
	[0-9])
		echo $IN is a single digit number
		;;

	[0-9][0-9])
		echo $IN is a 2 digit number
		;;
	*)
		echo Error
		;;
esac
