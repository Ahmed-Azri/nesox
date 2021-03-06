source nesox.sh

usage()
{
	echo "usage: startbeat.sh [servernode] [beaternode] [frequency]"
}

beaterport=8478
servernode=1
beaternode=3

if [ "$1" != "" ]; then servernode="$1"; fi
if [ "$2" != "" ]; then beaternode="$2"; fi

serverhost="$nodeprefix$servernode"
beaterhost="$nodeprefix$beaternode"

frequency=5
if [ "$3" != "" ]; then frequency="$3"; fi

size=104857600
size=1048576
size=10240
size=16384

scommand="nesox -l stats -g background -d $beat -r server -f $file $serverhost $beaterport"
bcommand="nesox -l stats -g background -d $beat -r beater -s $size $serverhost $beaterport $frequency"

sshscommand="ssh $user@$serverhost $scommand"
sshbcommand="ssh $user@$beaterhost $bcommand"

echo -e "${YELLOW}start heart beating server on $serverhost ${RESTORE}"
echo $sshscommand
coproc $sshscommand
wait $!
echo -e "${YELLOW}start heart beating beater on $beaterhost ${RESTORE}"
echo $sshbcommand
coproc $sshbcommand
wait $!


