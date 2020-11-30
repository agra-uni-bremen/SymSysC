#!/bin/bash
buildfolder="../build"
tests=("test_plic" "test_sensor" "test_sensor2")
today=date +"%d-%m-%Y"
mkdir test 2> /dev/null

for test in ${tests[@]}
do
	echo "Running test $test"
	testfolder="test/$test"
	mkdir $testfolder 2> /dev/null
	rm -r "$testfolder/*" 2> /dev/null
	make -C $buildfolder $test > "$testfolder/$today.log" 2>&1
done
