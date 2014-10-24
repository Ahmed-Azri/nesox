source nesox.sh

for node in $nodes
do
	echo "${BLUE}synctime on $node${RESTORE}"
	timestring=`date`
	shellcommand="date --set="$timestring""
	sshcommand="ssh $user@$node $shellcommand"
	coproc $sshcommand
	wait $!
done
