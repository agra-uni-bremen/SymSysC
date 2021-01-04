#!/bin/bash
buildfolder="build"
tests=("sensor" "sensor2" "uart" "plic#1" "plic#2")
today=$(date +"%Y-%m-%d-%H.%M")
klee_args=(
    "--libcxx"
    "--libc=uclibc"
    "-posix-runtime"
    #"--emit-all-errors=1"
    )

echo "Today is $today"
mkdir test 2> /dev/null
#	rm -r "$testfolder/*" 2> /dev/null

make -C $buildfolder clean
for test in ${tests[@]}
do
	base_name=$(echo $test | cut -d "#" -f1)
	subtype=$(echo $test | cut -d "#" -f2)
	testfolder=test/$test
	echo "Running test $base_name ($subtype)"
	mkdir "$testfolder" 2> /dev/null
	make -C $buildfolder testbench_$base_name
	{ time klee ${klee_args[*]} $buildfolder/testbench_$base_name $subtype ; } > "$testfolder/$today.log" 2>&1
	mkdir "$testfolder/$today"
	cp -rL $buildfolder/klee-last/* "$testfolder/$today"
done
