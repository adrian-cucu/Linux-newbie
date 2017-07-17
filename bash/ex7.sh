#!/bin/bash

read -p "Enter your name: " Name

hour=`date +"%H"`
: '
for test
hour=0
'
if   [[ $hour -ge 0  && $hour -lt 10 ]]; then

	echo Good morning \"$Name\"

elif [[ $hour -ge 10 && $hour -lt 20 ]]; then

	echo Good afernoon \"$Name\"

else

	echo Good evening \"$Name\"

fi
