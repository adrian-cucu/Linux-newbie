
#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Usage '${0##*/}' <string> "
	exit 1
fi

if [ -d $1 ]; then
	echo "'$1' is a subdirectory" 
else
	echo "'$1' is NOT a subdirectory"
fi
