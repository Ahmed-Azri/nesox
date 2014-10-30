source nesox.sh

cd $home/stat

statdatafiles=*.data

for statdatafile in $statdatafiles
do
	echo $statdatafile
	loadname=`getloadnamefromdatafile $statdatafile`
	for comparg in $(seq 0 3)
	do
		statname="compute-$comparg-$loadname"
		compute < $statdatafile 2> $statname.stat
	done
done
