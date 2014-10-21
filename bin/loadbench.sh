source nesox.sh


nodeprefix="202.45.128.17"
portprefix="817"

randnum=$RANDOM

processloadfile()
{
loadfile="$1"
while read line
do
	s="${line:0:1}"
	d="${line:2:1}"
	datasize="${line:4:4}"
	delaysec="${line:9:1}"
	printf "%s->%d:%s:%s\n" $s $d $datasize $delaysec
	snode="$nodeprefix$s"
	dnode="$nodeprefix$d"
	sport="$portprefix$d"
	delay="$delaysec$million"
	command="nesox -g back -r reader -s $datasize $snode $sport $delay"
	echo -e "${PURPLE}$command${RESTORE}"
done < $loadfile
}

all2all()
{
datasize="$1"
delay="$2$million"

echo -e "${PURPLE}all-to-all work load${RESTORE}"
for dnode in $nodes
do
	for snode in $nodes
	do
		sport="8${dnode:11}"
		printf "%s:%s %s\n" $snode $sport $dnode
		command="nesox -g back -r reader -s $datasize $snode $sport $delay"
		echo $command
		x="ssh $user@$dnode $command"
	done
done
}

while getopts ":f:a" opt;
do
	case $opt in
	("f")
		loadfile=$OPTARG
		loadengine=processloadfile
	;;
	(":")
		echo "Option -$OPTARG requires an argument." >&2
		exit 1
	;;
	("a")
		loadfile=""
		loadengine=all2all
	;;
	("?")
		echo "Invalid option: -$OPTARG" >&2
		exit 1
	;;
	esac
done
shift $((OPTIND - 1))

datasize="$1"
delay="$2$million"

$loadengine $loadfile $datasize $delay

