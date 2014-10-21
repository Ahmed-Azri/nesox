Bash Scripts for Nesox Cluster Benchmarking
===========================================

**should be self-contained and independent of other scripts**

operations on d0:
-----------------
git pull
checkcluster.sh
build
startcluster.sh
loadbench.sh -f sample.ld
collectdata.sh [1|2]
stopcluster.sh

operations on mac:
------------------
getstat.sh
cd stat2 (stat1)
aggregate.sh
cd ..

cd stat
compute.sh
cd ..
