#!/bin/bash

cores=(2 4 8) #12 24 30)
CPU_dim=(4 5 6 7)
runs=5
output="cpu-scale-eval.txt"
app="./cpu"

rm -fr $output

for d in ${CPU_dim[@]};do
	for c in ${cores[@]};do
		echo $d" "$c
		sum=0
		for i in $(seq $runs);do
			echo "run: "$i"/"$runs
			time=`$app $d $d $c | grep Time | awk '{print $2}'`
			sum=`echo $time + $sum | bc`
		done
		avg=`echo "scale=4;$sum / $runs" | bc`
		echo $d" "$d" "$c" "$avg >> $output
	done
done
