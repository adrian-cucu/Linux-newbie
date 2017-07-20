#!/bin/bash

if [[ $# -lt 1 ]]; then
	echo -e "USAGE \n\t${0##**/} -p <person name> | -i <N> <M>"
	exit 1
fi

case "$1" in
	-p)
			if [[ $# -lt 2 ]]; then
				echo Specify the name of the person...
				exit 1
			fi

			person_name="$2"
			found=0

			while read line; do
				if [[ "$line" =~ .*$person_name.* ]]; then
					if [[ $found -ne 0 ]]; then
						echo $line >> "$person_name.txt"
					else
						echo $line >  "$person_name.txt"
					fi
					found=1
				fi 	
			done < Agenda

			if (($found)); then
				printf "@@@@@(\33[32m$person_name\33[00m):\n"
				cat "$person_name.txt"
			else
				echo Not found
			fi	
		;;

	-i)
			if [[ $# -lt 3 ]]; then
				echo Need to specify [N, M] interval
				exit 1
			fi

			if [[ $2 -le $3	 ]]; then
				min=$2
				max=$3
			else
				min=$3
				max=$2
			fi

			found=0
			
			while read line; do
				num=${line%%:*}
				if [[ $num -ge $min && $num -le $max ]]; then
					if (($found)); then
						echo $line >> "Agenda-$min-$max.txt"
					else
						echo $line >  "Agenda-$min-$max.txt"
					fi
					found=1
				fi
			done < Agenda

			if (($found)); then
				cat "Agenda-$min-$max.txt"
			else
				echo Not found
			fi	
		;;
esac
