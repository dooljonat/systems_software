#!/bin/bash

cmd=${1}

initialize () {
  mkdir contents
	echo "hello, world!" > contents/hello
	mkdir contents/destdir
	echo "0123456789012 should not appear" > contents/destdir/destfile
	cd contents
}

cleanup () {
	echo "cleanup"
	cd ../
	rm -rf contents
}

initialize
trap "cleanup" EXIT SIGHUP SIGINT SIGQUIT SIGABRT SIGALRM SIGTERM

$cmd hello destdir/destfile
exitcode=$?
test -e destdir/destfile
moved=$?
echo $moved
diff <(echo "hello, world!") destdir/destfile
content=$?
echo $content

if [[ $exitcode -eq 0 && $moved -eq 0 && $content -eq 0 ]]; then
	echo "success"
	exit 0
else
	echo "failure"
	exit 1
fi
