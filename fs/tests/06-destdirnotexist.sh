#!/bin/bash

cmd=${1}

initialize () {
  mkdir contents
  touch contents/source_file
  cd contents
}

cleanup () {
	cd ../
	rm -rf contents
}

initialize
trap "cleanup" EXIT SIGHUP SIGINT SIGQUIT SIGABRT SIGALRM SIGTERM

TARGET="nonexistent_path/something"

$cmd source_file $TARGET
exitcode=$?

if [ $exitcode -eq 6 ]; then
	echo "success"
	exit 0
else
	echo "failure"
	exit 1
fi
