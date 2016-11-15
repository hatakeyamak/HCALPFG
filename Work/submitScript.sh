#!/bin/sh
cd $PBS_O_WORKDIR
hostname
source ~cmssoft/shrc
eval `scramv1 runtime -sh`

./run "InputFile/$filename.txt" "$filename" "OutPut" "$outStr" "0"
