source nesox.sh

cd $ldlg
loadnames=*

function parsereader()
{
	if [ "$1" = "" ]; then echo "parsereader: log file name needed"; exit 1; fi;
	if [ "$2" = "" ]; then echo "parsereader: target file name needed"; exit 1; fi;

	logfilename="$1"
	aggregatefile="$2"

	while read line
	do
		echo $line >> $aggregatefile
	done < $logfilename
}


for loadname in $loadnames
do
	echo -e "${YELLOW}aggregating load: $loadname${RESTORE}"
	aggregatefile="$stat/$loadname.data"
	cd $loadname
	logfilenames=nesox-reader*.log
	for logfilename in $logfilenames
	do
		parsereader $logfilename $aggregatefile
	done
done

