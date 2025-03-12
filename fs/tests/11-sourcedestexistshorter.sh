#!/bin/bash

cmd=${1}

initialize () {
  mkdir contents
	echo "hello, world!" > contents/hello
	echo "0123456789" > contents/destfile
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
diff <(echo "hello, world!") destfile
content=$?
echo $content

if [[ $exitcode -eq 0 && $moved -eq 0 && $content -eq 0 ]]; then
	echo "success"
	exit 0
else
	echo "failure"
	exit 1
fi
