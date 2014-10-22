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
loadbench.sh -a 102400 5 [portfamily (default: 8)]
loadbench.sh -f load01.ld [portfamily (default: 8)]
collectdata.sh
stopcluster.sh
checkcluster.sh

operations on mac:
------------------
getstat.sh 2 (default: stat1)
cd stat2 (stat1)
aggregate.sh
cd ..

cd stat
compute.sh
cd ..
