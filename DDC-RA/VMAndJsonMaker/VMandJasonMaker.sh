#!/bin/bash

# this bash generate the VM configuration and make Json file for execution
#You need both vm-generator.sh and maker.sh to mak ethis script work properly
#Getting the current directy that this script exist
SOURCE="${BASH_SOURCE[0]}"
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"

NUMBEROFVM=1000
NUMBEROFCLUSTER=100

#echo "generating VMs ..."

cmd="$DIR"/./vm-generator.sh" $NUMBEROFVM"
eval $cmd

echo "generating Json file"

cmd="$DIR"/./maker.sh" $NUMBEROFCLUSTER"
eval $cmd

echo "moveing Janon file"
cp testJson.json ../input.json

#removing files
#cmd="rm $DIR/*.txt *.json"
#eval $cmd
