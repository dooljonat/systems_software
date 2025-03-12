#!/bin/bash

cmd=${1}

initialize () {
  mkdir contents
	touch contents/hello
	mkdir contents/dest
	cd contents
}

cleanup () {
	echo "cleanup"
	cd ../
	rm -rf contents
}

initialize
trap "cleanup" EXIT SIGHUP SIGINT SIGQUIT SIGABRT SIGALRM SIGTERM

$cmd hello dest
exitcode=$?

if [[ $exitcode -eq 1 ]]; then
	echo "success"
	exit 0
else
	echo "failure"
	exit 1
fi
