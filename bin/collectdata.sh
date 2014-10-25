source nesox.sh

for node in $nodes
do
	echo -e "${RED}collect nesox data from $node ... ${RESTORE}"
	sshcommand="scp -r $user@$node:$ldlg/* $ldlg/"
	coproc $sshcommand
	wait $!
done
