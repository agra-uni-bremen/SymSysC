#!/bin/bash
buildfolder="build"
tests=(
    #"sensor#1"
    #"sensor#2"
    #"uart"
    "plic#functional_test_basic"
    "plic#functional_test_consider_threshold"
    "plic#functional_test_priority_direct"
    "plic#interface_test_read"
    "plic#interface_test_write"
    "plic_fault#functional_test_basic"          #Ih, copypasta
    "plic_fault#functional_test_consider_threshold"
    "plic_fault#functional_test_priority_direct"
    "plic_fault#interface_test_read"
    "plic_fault#interface_test_write"
    "vanilla_plic#functional_test_basic"          #Ih, copypasta
    "vanilla_plic#functional_test_consider_threshold"
    "vanilla_plic#functional_test_priority_direct"
    "vanilla_plic#interface_test_read"
    "vanilla_plic#interface_test_write"
    )
today=$(date +"%Y-%m-%d-%H.%M")
testfolder_base=test/$today
klee_args=(
    "--libcxx"
    "--libc=uclibc"
    "-posix-runtime"
    "-only-output-states-covering-new"
    #"-max-memory=40000"	#default: 2000 -> 2GB
    #"--emit-all-errors=1"
    )
klee_vanilla_args=(
    "-link-llvm-lib=../source/systemc-dist/lib_llvm/libsystemc.so"
    "--libcxx"
    )

echo "Today is $today, writing to $testfolder_base"
#	rm -r "$testfolder/*" 2> /dev/null
mkdir -p $testfolder_base 2> /dev/null

make -s -C $buildfolder clean
i=0 #lol
for test in ${tests[@]}
do
	base_name=$(echo $test | cut -d "#" -f1)
	subtype=$(echo $test | cut -d "#" -s -f2)
	testfolder=$testfolder_base/$test
	echo "Building testbench_$base_name"
	mkdir "$testfolder" 2> /dev/null
	make -C $buildfolder testbench_$base_name --no-print-directory
	echo "Running test $base_name ($subtype)"
	if [[ $base_name == *"vanilla"* ]]; then
        echo "Using vanilla SysC args"
        args=klee_vanilla_args
    else
        echo "Using normal klee args"
        args=klee_args
    fi
	{ time klee ${args[*]} $buildfolder/testbench_$base_name $subtype ; } > "$testfolder/run.log" 2>&1 &
	klee_pid[${i}]=$!
	sleep 1
	klee_folder[${i}]=$(readlink -f $buildfolder/klee-last)
	klee_target_folder[${i}]=$testfolder
	echo "$base_name ($subtype) running as ${klee_pid[${i}]} into ${klee_folder[${i}]}"
	i=$[i + 1]
done

echo "All processes started. Waiting for processes to finish..."

for ((i=0;i<${#tests[@]};i++)); do
    echo "waiting for PID ${klee_pid[${i}]} (${tests[${i}]})..."
    wait "${klee_pid[${i}]}"
    echo "${tests[${i}]} finished."
    mv "${klee_folder[${i}]}" "${klee_target_folder[${i}]}"
done

end_stats=$testfolder_base/klee-stat.log

klee-stats --print-more "$testfolder_base"/* > "$end_stats"
#tail $testfolder_base/*/run.log >> $testfolder_base/klee-stat.log

for ((i=0;i<${#tests[@]};i++)); do
    echo >> "$end_stats"
    echo "${klee_target_folder[${i}]}/run.log found errors: " >> "$end_stats"
    cat "${klee_target_folder[${i}]}/run.log" | grep ERROR >> "$end_stats"
    tail -n 4 "${klee_target_folder[${i}]}/run.log" >> "$end_stats"
done

cat $testfolder_base/klee-stat.log
