#!/bin/bash

cpu_file_link="http://azurepublicdatasetv2.blob.core.windows.net/azurepublicdatasetv2/azure2019_data/cores.txt"
memory_file_link="http://azurepublicdatasetv2.blob.core.windows.net/azurepublicdatasetv2/azure2019_data/memory.txt"
cpu_file="cores.txt"
mem_file="memory.txt"

number_vms=$1
local_mem_sla_ratio=0.1

vm_cpu_file="vm_cores.txt"
vm_mem_file="vm_mem.txt"
vm_mem_local_sla_file="vm_mem_local_sla.txt"
vm_mem_remote_sla_file="vm_mem_remote_sla.txt"

coeff=$(($number_vms / 100))

rm -f $cpu_file $mem_file
wget $cpu_file_link
`awk -v coeff=$coeff '{if(NR!=1){print $1" "$2*coeff}}' $cpu_file | awk '{while ($2--) print $1}' | shuf > $vm_cpu_file`

wget $memory_file_link
`awk -v coeff=$coeff '{if(NR!=1){print $1" "$2*coeff}}' $mem_file | awk '{while ($2--) print $1*1024}' | shuf > $vm_mem_file`

`awk -v sla=$local_mem_sla_ratio '{print int($1*sla)}' $vm_mem_file > $vm_mem_local_sla_file`

paste $vm_mem_file $vm_mem_local_sla_file > mem_info.txt

while IFS="	" read -r m l remain; do
	random_number=`bc <<< "scale=2; ${RANDOM}/32767"`
	remote_constant=`echo 1 + $random_number | bc`
	echo $remote_constant >> remote_constant.txt
	r=$(($m-$l))
	r_sla=$(echo $remote_constant*$r | bc)
	echo ${r_sla%.*}
done < "mem_info.txt" > $vm_mem_remote_sla_file



#echo "# id core_number mem_size(MB) local_mem_sla (MB) remote_mem_sla(MB) remote_constant" > vm.txt

echo > vm.txt

`awk '{print NR}' $vm_mem_file > index.txt`

paste index.txt $vm_cpu_file $vm_mem_file $vm_mem_local_sla_file $vm_mem_remote_sla_file remote_constant.txt | head -n $number_vms > vm.txt

head -n 21 vm.txt > vm2.txt

rm -f $cpu_file $mem_file $vm_cpu_file $vm_mem_file $vm_cpu_file $vm_mem_file $cpu_file $mem_file $vm_mem_local_sla_file index.txt header.txt mem_info.txt $vm_mem_remote_sla_file remote_constant.txt
