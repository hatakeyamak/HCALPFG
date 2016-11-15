#!/bin/sh

submitscript=submitScript.sh

outStr=$1

mkdir -p qsub


  for runnum in 278963to279667; do # 275311 277305 277420 278018 278240 278406 279479 279588; do

    njobs=`ls InputFile_ForMany/run${runnum}_allPhi_* | wc -l`
    echo number of jobs: $njobs

      for i in `seq 0 $njobs`; do

        export filenum=$i
        export outStr=$outStr
        export runnum=$runnum
        echo $filenum
        echo $runnum
        #qsub -l walltime=12:00:00 -N $i_$outStr -o qsub/ -e qsub/ -V $submitscript  -q moonshot
        qsub -l walltime=12:00:00 -N $i_$outStr -o qsub/ -e qsub/ -V $submitscript

        sleep 1

      done
  done



