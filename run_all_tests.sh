#!/bin/bash
buildfolder="build"
tests=("sensor" "sensor2" "uart" "plic#1" "plic#2" "plic#3")
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
	echo "Running test $base_name ($subtype)"
	mkdir "$testfolder" 2> /dev/null
	make -C $buildfolder testbench_$base_name --no-print-directory
	{ time klee ${klee_args[*]} $buildfolder/testbench_$base_name $subtype ; } > "$testfolder/run.log" 2>&1
	mkdir "$testfolder/$today"
	cp -rL $buildfolder/klee-last/* "$testfolder/$today"
done
klee-stats $testfolder_base/* > $testfolder_base/klee-stat.log
tail $testfolder_base/*/run.log >> $testfolder_base/klee-stat.log

cat $testfolder_base/klee-stat.log
