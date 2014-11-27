source nesox.sh

usage()
{
	echo "Usage: loadtrans.sh trans.ld"
	exit
}

processloadfile()
{
loadfile="$1"
portfamily="8"
loadname=`getloadnamefromloadfile $loadfile`
loadhome="$ldlg/$loadname"

while read line
do
	s="${line:0:1}"
	d="${line:2:1}"
	datasize="${line:6}"
	delaysec="${line:4:1}"
	printf "%s->%d:%s:%s\n" $s $d $datasize $delaysec
	snode="$nodeprefix$s"
	dnode="$nodeprefix$d"
	sport="${portfamily:0:1}${portprefix:1:2}$d"
	delay="$delaysec$million"

	shellcommand="if [ -e $loadhome ]; then echo 'exits!'; else mkdir -p $loadhome; fi"
	sshcommand="ssh $user@$dnode $shellcommand"
	echo -e "${PURPLE}$sshcommand${RESTORE}"
	coproc $sshcommand
	wait $!

	shellcommand="nesox -g back -r reader -d $loadhome -s $datasize $snode $sport $delay"
	sshcommand="ssh $user@$dnode $shellcommand"
	echo -e "${PURPLE}$sshcommand${RESTORE}"
	coproc $sshcommand
	wait $!
done < $loadfile
}

if [ "$1" = "" ]; then usage; else loadfile="$1"; fi

processloadfile $loadfile

