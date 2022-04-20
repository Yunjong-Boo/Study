#!/bin/bash

POLICIES=("FIFO" "LRU" "OPT" "RAND" "CLOCK")

for policy in "${POLICIES[@]}"
do
	echo "policy: $policy"
	for i in 1 2 3 4
	do
		echo "cache size: ${i}"
		python3 paging-policy.py -N -c -f vpn.txt -p "$policy" --cachesize="$i"
	done
done
