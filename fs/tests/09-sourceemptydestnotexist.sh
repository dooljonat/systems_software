#!/bin/bash

cmd=${1}

initialize () {
  mkdir contents
	touch contents/hello
	cd contents
}

cleanup () {
	echo "cleanup"
	cd ../
	rm -rf contents
}

initialize
trap "cleanup" EXIT SIGHUP SIGINT SIGQUIT SIGABRT SIGALRM SIGTERM

$cmd hello destfile
exitcode=$?
test -e destfile
moved=$?
echo $moved

if [[ $exitcode -eq 0 && $moved -eq 0 ]]; then
	echo "success"
	exit 0
else
	echo "failure"
	exit 1
fi
