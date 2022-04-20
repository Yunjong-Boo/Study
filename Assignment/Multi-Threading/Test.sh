#!/bin/bash

STANDARD_SIZE=64
while [ $STANDARD_SIZE -le 4096 ]
do
	perf stat -e task-clock,context-switches,cache-references,cache-misses,L1-dcache-load-misses,L1-dcache-loads,l2_rqsts.demand_data_rd_hit,l2_rqsts.demand_data_rd_miss,LLC-loads,LLC-load-misses,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,iTLB-loads,iTLB-load-misses ./optimize-queue A_8192 B_8192 $STANDARD_SIZE >> performance.txt
	STANDARD_SIZE=$[ $STANDARD_SIZE * 2 ]
done
