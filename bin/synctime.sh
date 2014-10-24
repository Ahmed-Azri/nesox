source nesox.sh

for node in $nodes
do
	timestring=`date`
	shellcommand="date -s "$timestring""
	sshcommand="ssh $user@$node $shellcommand"
	coproc $sshcommand
	wait $!
done
