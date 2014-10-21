source nesox.sh

loadengine=usage

usage()
{
	echo "usage: loadbench.sh [options] [size] [delay]"
	echo "options:"
	echo "  -f loadfile.ld"
	echo "  -a (all2all)"
	echo
	echo "notice:"
	echo "  size: data to transfer in bytes for -a option"
	echo " delay: data to transfer in bytes for -a option"
	echo
	echo "example:"
	echo "loadengine.sh -a size delay"
	echo "contact: liying.hku@gmail.com"
	echo
}

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
	command="nesox -g back -r reader -d $home -s $datasize $snode $sport $delay"
	sshcommand="ssh $user@$dnode $command"
	echo -e "${PURPLE}$sshcommand${RESTORE}"
	$sshcommand
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
		command="nesox -g back -r reader -d $home -s $datasize $snode $sport $delay"
		sshcommand="ssh $user@$dnode $command"
		echo -e "${PURPLE}$sshcommand${RESTORE}"
		$sshcommand
	done
done
}

while getopts ":f:a" opt;
do
	case $opt in
	("f")
		loadengine=processloadfile
		loadfile=$OPTARG
	;;
	("a")
		loadengine=all2all
		loadfile=""
	;;
	(":")
		echo "Option -$OPTARG requires an argument." >&2
		exit 1
	;;
	("?")
		echo "Invalid option: -$OPTARG" >&2
		exit 1
	;;
	esac
done
shift $((OPTIND - 1))

if [ "$1" = "" ];
then
datasize=1024
else
datasize="$1"
fi

delay="$2"

$loadengine $loadfile $datasize $delay
