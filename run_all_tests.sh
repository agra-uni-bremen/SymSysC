#!/bin/bash
buildfolder="build"
tests=("test_sensor" "test_sensor2" "test_uart" "test_plic")
today=$(date +"%Y-%m-%d-%H.%M")

echo "Today is $today"
mkdir test 2> /dev/null
#	rm -r "$testfolder/*" 2> /dev/null

for test in ${tests[@]}
do
	echo "Running test $test"
	testfolder="test/$test"
	mkdir $testfolder 2> /dev/null
	make -C $buildfolder $test > "$testfolder/$today.log" 2>&1
	mkdir "$testfolder/$today"
	cp -rL $buildfolder/klee-last/* $testfolder/$today
done
