source nesox.sh

for node in $nodes
do
	echo -e "${RED}collect nesox data from $node ... ${RESTORE}"
	sshcommand1="scp -r $user@$node:$logs $home/sta1t/$node-logs"
	sshcommand2="scp -r $user@$node:$logs/* $home/stat2/"
	coproc $sshcommand1
	wait $!
	coproc $sshcommand2
	wait $!
done

