source nesox.sh

for node in $nodes
do
	echo -e "${BLUE}synctime on $node${RESTORE}"
	timestring=`date`
	shellcommand="date --set="'"'"$timestring"'"'
	sshcommand="ssh $user@$node $shellcommand"
	echo $sshcommand
	coproc $sshcommand
	wait $!
done
