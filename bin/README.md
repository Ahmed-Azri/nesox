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

for each load
	on d0: loadbench.sh -f load.ld
	wait for some time

on d0: collectdata.sh
on d0: aggregate.sh
on d0: compute.sh

on mac: getstat.sh
