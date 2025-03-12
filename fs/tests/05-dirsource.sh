#!/bin/bash

cmd=${1}

initialize () {
  mkdir contents
	mkdir contents/dirfile
	cd contents
}

cleanup () {
	cd ../
	rm -rf contents
}

initialize
trap "cleanup" EXIT SIGHUP SIGINT SIGQUIT SIGABRT SIGALRM SIGTERM

$cmd dirfile target
exitcode=$?

if [ $exitcode -eq 5 ]; then
	echo "success"
	exit 0
else
	echo "failure"
	exit 1
fi
