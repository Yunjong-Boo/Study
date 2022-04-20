#!/bin/bash

num_pages=1
make
while [ $num_pages -le 8192 ]
do
	./tlb $num_pages 50000 >> data.txt
	num_pages=$[ $num_pages * 2 ]
done
python3 ./Graph.py
rm data.txt
eog Graph.png
