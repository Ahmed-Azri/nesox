source nesox.sh

for node in $nodes
do
	echo -e "${RED}collect nesox beat data from $node ... ${RESTORE}"
	sshcommand="scp -r $user@$node:$beat/* $beat/"
	coproc $sshcommand
	wait $!
done
