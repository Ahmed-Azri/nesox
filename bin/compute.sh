source nesox.sh

cd $home/stat

statdatafiles=*.data

function getloadnamefromdatafile()
{
	datafile="$1"
	datafilelength=${#datafile}
	datafilelength=$(expr $datafilelength - 5)
	loadname=${datafile:0:datafilelength}
	echo $loadname
}

for statdatafile in $statdatafiles
do
	echo $statdatafile
	loadname=`getloadnamefromdatafile $statdatafile`
	compute < $statdatafile 2> $loadname.stat
done
