source nesox.sh

cd $stat
statdatafiles=*.data

for statdatafile in $statdatafiles
do
	echo $statdatafile
	loadname=`getloadnamefromdatafile $statdatafile`
	for comparg in $(seq 0 3)
	do
		postfix="com$comparg"
		compute $comparg < $statdatafile 2> $loadname.$postfix
	done
done
