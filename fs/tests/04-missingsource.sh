#!/bin/bash

cmd=${1}

# Define arguments
ARG1="weoih3g9ihergierhg9erghe9rghu9gh349ghe4rg934gh34g.txt"
ARG2="3pit43h4t904w85yt0894tyh4598th354t98gehgbe4i.txt"

$cmd $ARG1 $ARG2


exitcode=$?

if [ $exitcode -eq 4 ]; then
	echo "success"
	exit 0
else
	echo "failure"
	exit 1
fi
