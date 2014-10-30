source nesox.sh

cd $stat
statdatafiles=*.data

for statdatafile in $statdatafiles
do
	echo $statdatafile
	loadname=`getloadnamefromdatafile $statdatafile`
	for comparg in $(seq 0 3)
	do
		statname="compute-$comparg-$loadname"
		compute $comparg < $statdatafile 2> $statname.stat
	done
done
