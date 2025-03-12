#!/bin/bash

echo "${0}"

cmd=${1}

testcmd="ls"
outfile="/dev/null"

initialize () {
  mkdir testdir
	cd testdir
	mkdir contents
	cd contents
	touch testfile1
	touch testfile2
	${testcmd} >${outfile}
}

cleanup () {
	cd ../
	cd ../
	rm -rf testdir
}

initialize
trap "cleanup" EXIT SIGHUP SIGINT SIGQUIT SIGABRT SIGALRM SIGTERM

echo "${testcmd} >${outfile}" | timeout 2 $cmd | sed 's/$ //g' | sed '/^$/d' > ../test.out
diff ../test.out /dev/null
exitcode=$?

if [[ $exitcode -eq 0 ]]; then
	echo "success"
	exit 0
else
	echo "failure"
	exit 1
fi
