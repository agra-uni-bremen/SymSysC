#!/bin/bash
buildfolder="build"
tests=(
    "sensor#1"
    "sensor#2"
    "uart"
    "plic#1"
    "plic#2"
    "plic#3"
    "plic#4"
    "plic#5"
    )
today=$(date +"%Y-%m-%d-%H.%M")
testfolder_base=test/$today
klee_args=(
    "--libcxx"
    "--libc=uclibc"
    "-posix-runtime"
    "-max-memory=40000"	#default: 2000 -> 2GB
    #"--emit-all-errors=1"
    )

echo "Today is $today, writing to $testfolder_base"
#	rm -r "$testfolder/*" 2> /dev/null
mkdir -p $testfolder_base 2> /dev/null

make -s -C $buildfolder clean
for test in ${tests[@]}
do
	base_name=$(echo $test | cut -d "#" -f1)
	subtype=$(echo $test | cut -d "#" -s -f2)
	testfolder=$testfolder_base/$test
	echo "Building testbench_$base_name"
	mkdir "$testfolder" 2> /dev/null
	make -C $buildfolder testbench_$base_name --no-print-directory
	echo "Running test $base_name ($subtype)"
	{ time klee ${klee_args[*]} $buildfolder/testbench_$base_name $subtype ; } > "$testfolder/run.log" 2>&1
	mkdir "$testfolder/$today"
	current_klee_folder=$(readlink -f $buildfolder/klee-last)
	mv "$current_klee_folder" "$testfolder/"
done
klee-stats $testfolder_base/* > $testfolder_base/klee-stat.log
tail $testfolder_base/*/run.log >> $testfolder_base/klee-stat.log

cat $testfolder_base/klee-stat.log
