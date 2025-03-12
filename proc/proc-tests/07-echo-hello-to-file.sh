#!/bin/bash

echo "${0}"

cmd=${1}

testcmd="echo hello"
outfile="out.txt"

initialize () {
  mkdir testdir
	cd testdir
	mkdir contents
	cd contents
	${testcmd} > ../groundtruth.out
}

cleanup () {
	cd ../
	cd ../
	rm -rf testdir
}

initialize
trap "cleanup" EXIT SIGHUP SIGINT SIGQUIT SIGABRT SIGALRM SIGTERM

echo "${testcmd} >${outfile}" | timeout 2 $cmd | sed 's/$ //g' | sed '/^$/d' > ../test.out
diff "${outfile}" ../groundtruth.out
exitcode=$?

if [[ $exitcode -eq 0 ]]; then
	echo "success"
	exit 0
else
	echo "failure"
	exit 1
fi
