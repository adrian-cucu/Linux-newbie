#!/bin/bash

read -p "Enter a number: " NUM

echo $NUM

if [[ $NUM == [1-9][1-9] ]] || 
   [[ $NUM == [1-9]      ]];
 then
	printf "\33[32m""OK""\33[00m\n"

else
	printf "\33[31m""NOT OK""\33[00m\n"

fi
