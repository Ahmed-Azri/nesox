source nesox.sh

loadengine=usage

usage()
{
	echo "usage: loadbench.sh [options] [size] [delay] [portfamily]"
	echo "options:"
	echo "  -f loadfile.ld"
	echo "  -a (all2all)"
	echo
	echo "notice:"
	echo "  size: data to transfer in bytes for -a option"
	echo " delay: data to transfer in bytes for -a option"
	echo "portfm: port family: 6, 7, 8, 9 (default:8)"
	echo
	echo "example:"
	echo "loadengine.sh -a size delay"
	echo "contact: liying.hku@gmail.com"
	echo
}

randnum=$RANDOM

processloadfile()
{
loadfile="$1"
portfamily="$2"

while read line
do
	s="${line:0:1}"
	d="${line:2:1}"
	datasize="${line:4:4}"
	delaysec="${line:9:1}"
	printf "%s->%d:%s:%s\n" $s $d $datasize $delaysec
	snode="$nodeprefix$s"
	dnode="$nodeprefix$d"
	sport="${portfamily:0:1}${portprefix:1:2}$d"
	delay="$delaysec$million"
	shellcommand="nesox -g back -r reader -d $home -s $datasize $snode $sport $delay"
	sshcommand="ssh $user@$dnode $shellcommand"
	echo -e "${PURPLE}$sshcommand${RESTORE}"
	coproc $sshcommand
	wait $!
done < $loadfile
}

all2all()
{
datasize="$1"
delay="$2$million"
portfamily="$3"

echo -e "${PURPLE}all-to-all work load${RESTORE}"
for dnode in $nodes
do
	for snode in $nodes
	do
		sport="${portfamily:0:1}${dnode:11}"
		shellcommand="nesox -g back -r reader -d $home -s $datasize $snode $sport $delay"
		sshcommand="ssh $user@$dnode $shellcommand"
		echo -e "${PURPLE}$sshcommand${RESTORE}"
		coproc $sshcommand
		wait $!
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

if [ "$2" = "" ];
then
delay=0
else
delay="$2"
fi

if [ "$3" = "" ];
then
portfamily=8
else
portfamily="$3"
fi

$loadengine $loadfile $datasize $delay $portfamily


