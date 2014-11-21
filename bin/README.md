Bash Scripts for Nesox Cluster Benchmarking
===========================================

**should be self-contained and independent of other scripts**

operations on d0:
-----------------
clean
git pull
cleancluster.sh
checkcluster.sh
build
deploy
startcluster.sh
loadbench.sh -f loadname.ld [portfamily (default: 8)]
collectdata.sh
stopcluster.sh
checkcluster.sh

operations on mac: (obsolete)
------------------
getstat.sh 2 (default: stat1)
cd stat2 (stat1)
aggregate.sh loadname
cd ..

cd stat
compute.sh
cd ..



data collection procedure
-------------------------

on mac: design and express featured work load into buildload.sh
on mac: execute buildload.sh

32 randomized workload is going to be constant!!! (naming: norm-variance-mean-timeinstant)

manually configuration should be named in (manu-num.ld) (for num in `seq -1 01 - 10`)


on d0: loadall.sh (loadall.sh 120 8: run in interactive mode for OF!!!)
on d0: collectdata.sh
on d0: aggregate.sh
on d0: compute.sh
on d0: generatestat.sh

on mac: getdata.sh
on mac: getstat.sh


manually manage and archive statistical data in terms of algorithms
-------------------------------------------------------------------



