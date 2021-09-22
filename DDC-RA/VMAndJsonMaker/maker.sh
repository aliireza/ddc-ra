#!/bin/bash


jsonFile="testJson.json"

rm -f $jsonFile

#Number of cluster 
ncluster=$1 # input args 

#number of CPU in each pool
nCPUinCpuPool=$((4 * 28))
nCPUinLOCALMEM=0
nCPUinREMOTEMEM=0

#memory access penalty of eac pool
localPenalty=0
localRemotePenalty=1
remoteRemotePenalty=2

#memeory size of each pool
memSizeCPUPool=$((4 * 8 * 1024))
memSizeLOCALMEM=$((4 * 32 * 1024))
memSizeREMOTEMEM=$((4* 64 * 1024))

#starting the json format
echo { >> $jsonFile
	#starting pools
  echo \"pools\": [ >> $jsonFile

for cluster in `eval echo {1..$ncluster}`
do

echo { >> $jsonFile
echo      \"type\": \"CPU\", >> $jsonFile
echo      \"cores\": $nCPUinCpuPool, >> $jsonFile
echo      \"memory\": $memSizeCPUPool, >> $jsonFile
echo      \"penalty\": $localPenalty >> $jsonFile
echo    }, >> $jsonFile
echo    { >> $jsonFile
echo      \"type\": \"LOCALMEM\", >> $jsonFile
echo      \"cores\": $nCPUinLOCALMEM, >> $jsonFile
echo      \"memory\": $memSizeLOCALMEM, >> $jsonFile
echo      \"penalty\": $localRemotePenalty >> $jsonFile
echo    }, >> $jsonFile 
echo    { >> $jsonFile 
echo      \"type\": \"REMOTEMEM\", >> $jsonFile
echo      \"cores\": $nCPUinREMOTEMEM, >> $jsonFile
echo      \"memory\": $memSizeREMOTEMEM, >> $jsonFile
echo      \"penalty\": $remoteRemotePenalty >> $jsonFile
echo    }, >> $jsonFile

done 

 sed -i '$d' $jsonFile
 echo } >> $jsonFile

#starting pool assignment to each cluster
 echo ], >> $jsonFile
 echo \"clusters\": [ >> $jsonFile

 for cl in `eval echo {0..$(($ncluster-1))}`
 do
 echo	   { >> $jsonFile
 echo     \"pools\": [ >> $jsonFile
 echo       $(($cl * 3 + 1)), >> $jsonFile
 echo       $(($cl * 3 + 2)), >> $jsonFile
 echo       $(($cl * 3 + 3)) >> $jsonFile
 echo     ] >> $jsonFile
 echo   }, >> $jsonFile
 done

 sed -i '$d' $jsonFile
 echo } >> $jsonFile
 echo ], >> $jsonFile

#This part get information regarding logical servers
file="vm.txt"

echo       \"servers\": [ >> $jsonFile
while IFS="	" read -r id core memSize localSLA remoteSLA remotecoeff remaining
do
        # f1 #id f2 core_number f3 mem_size(GB)  
        #echo "$f1" "$f2" "$f3"
         # for each line we creat one fild
    echo { >> $jsonFile
    echo  \"cores\": $core, >> $jsonFile
    echo   \"mem\": $memSize, >> $jsonFile
    echo  \"local_SLA\": $localSLA, >> $jsonFile
    echo  \"remote_SLA\": $remoteSLA >> $jsonFile
    echo }, >> $jsonFile

done <"$file"

# to remove the camma from last line
sed -i '$d' $jsonFile
echo } >> $jsonFile
echo  ] >> $jsonFile # closing the server
echo } >> $jsonFile



