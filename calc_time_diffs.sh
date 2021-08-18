#!/bin/bash

starttime=$1
testfolder=$2
$(date +"%Y-%m-%d-%H.%M")

for testfile in $testfolder/*.ktest; do
	lastModificationSeconds=$(date +%s -r $testfile)
done
