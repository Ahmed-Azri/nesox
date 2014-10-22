source nesox.sh

command=stopnesox.sh

for node in $nodes
do
	echo -e "${BLUE}stop nesox on $node ... ${RESTORE}"
	echo $command
	ssh $user@$node $command
done

