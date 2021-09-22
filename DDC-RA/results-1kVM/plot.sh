#!/bin/bash

rm -f *.txt 

cluster_file_ff="cluster-data.csv.ff"
cluster_file_cp="cluster-data.csv.cp"

non_zero_clusters_cp=`awk -F, '{if($2!=0){print $0}}' $cluster_file_cp | wc -l`
non_zero_clusters_ff=`awk -F, '{if($2!=0){print $0}}' $cluster_file_ff | wc -l`

if [ $non_zero_clusters_cp -gt $non_zero_clusters_ff ]
then
	non_zero_clusters=$non_zero_clusters_cp
else
	non_zero_clusters=$non_zero_clusters_ff
fi

head -n $non_zero_clusters $cluster_file_cp > filtered_cluster_cp.txt
head -n $non_zero_clusters $cluster_file_ff > filtered_cluster_ff.txt

awk -F, '{if(NR!=1){print $9}}' logicalServer-data.csv.cp > price_cp.txt
awk -F, '{if(NR!=1){print $9}}' logicalServer-data.csv.ff > price_ff.txt
awk -F, '{if(NR!=1){print $11}}' logicalServer-data.csv.ff > slo_local_ff.txt
awk -F, '{if(NR!=1){print $12}}' logicalServer-data.csv.ff > slo_remote_ff.txt

echo "#CP_price,FF_price" > price_compare.txt
paste -d, price_cp.txt price_ff.txt >> price_compare.txt
awk -F, '{if(NR!=1){print $2-$1}}' price_compare.txt > price_delta.txt

echo "#CP_price,FF_price,FF-CP-delta,slo_local_ff,slo_remote_ff" > price_compare.txt
paste -d, price_cp.txt price_ff.txt price_delta.txt slo_local_ff.txt slo_remote_ff.txt >> price_compare.txt

`gnuplot plot.gp`