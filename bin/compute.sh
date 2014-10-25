source nesox.sh

cd $home/stat

statdatafiles=*.data

for statdatafile in $statdatafiles
do
	echo $statdatafile
	loadname=`getloadnamefromdatafile $statdatafile`
	compute < $statdatafile 2> $loadname.stat
done
