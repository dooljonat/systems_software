#!/bin/bash

echo "${0}"

cmd=${1}

testcmd="ls | wc -l"

initialize () {
  mkdir testdir
	cd testdir
	mkdir contents
	cd contents
	touch testfile1
	touch testfile2
}

cleanup () {
	cd ../
	cd ../
	rm -rf testdir
}

initialize
trap "cleanup" EXIT SIGHUP SIGINT SIGQUIT SIGABRT SIGALRM SIGTERM

echo "${testcmd}" | timeout 2 $cmd | sed 's/$ //g' | sed '/^$/d' > ../test.out
echo "${testcmd}" | bash | sed 's/$ //g' | sed '/^$/d' > ../groundtruth.out
cat ../test.out ../groundtruth.out
diff ../test.out ../groundtruth.out
exitcode=$?

if [[ $exitcode -eq 0 ]]; then
	echo "success"
	exit 0
else
	echo "failure"
	exit 1
fi
