#!/bin/bash
cmd=${1}

$cmd

if [ $? -eq 3 ]; then
	echo "success"
	exit 0
else
	echo "failure"
	exit 1
fi
