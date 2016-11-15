#!/bin/sh

submitscript=submitScript.sh

outStr=$1

mkdir -p qsub



    njobs=`ls InputFile/run*txt | wc -l`
    echo number of jobs: $njobs

      for i in `ls InputFile`; do

        export filename=$i
        export outStr=$outStr
        echo $filename
        qsub -l walltime=12:00:00 -N $i_$outStr -o qsub/ -e qsub/ -V $submitscript  

        sleep 1

      done

