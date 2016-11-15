#!/bin/sh
cd $PBS_O_WORKDIR
#hostname
source ~cmssoft/shrc
eval `scramv1 runtime -sh`


if [ $filenum -lt 10 ]
then
./runCal "InputFile_ForMany/run${runnum}_allPhi_0$filenum" "run${runnum}_allPhi_0$filenum" "OutPut" "$outStr" "0"
else
./runCal "InputFile_ForMany/run${runnum}_allPhi_$filenum" "run${runnum}_allPhi_$filenum" "OutPut" "$outStr" "0"
fi
