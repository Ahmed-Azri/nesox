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

operations on mac:
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

for each load
	on d0: loadbench.sh -f load.ld
	on d0: collectdata.sh
	goto: operations on mac

