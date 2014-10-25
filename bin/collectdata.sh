source nesox.sh

for node in $nodes
do
	echo -e "${RED}collect nesox data from $node ... ${RESTORE}"
	sshcommand1="scp -r $user@$node:$logs $home/stat1/$node-logs"
	sshcommand2="scp -r $user@$node:$logs/* $home/stat2/"
	sshcommand3="scp -r $user@$node:$ldlg/* $ldlg/"
	coproc $sshcommand1
	wait $!
	coproc $sshcommand2
	wait $!
	coproc $sshcommand3
	wait $!
done
