#!/bin/bash

read continent
read city

TZ="$continent/$city"
export TZ
hour=`date +"%H"`

: '
for test
hour=0
'

echo $hour

exit 0
if   [[ $hour -ge 0  && $hour -lt 10 ]]; then

	echo Good morning \"$Name\"

elif [[ $hour -ge 10 && $hour -lt 20 ]]; then

	echo Good afernoon \"$Name\"

else

	echo Good evening \"$Name\"

fi
