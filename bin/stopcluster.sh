source nesox.sh

command=stopnesox.sh

for node in $nodes
do
	echo -e "${RED}start nesox on $node ... ${RESTORE}"
	echo $command
	ssh $user@$node $command
done

