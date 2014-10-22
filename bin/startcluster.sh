source nesox.sh

command=startnesox.sh

for node in $nodes
do
	echo -e "${BLUE}start nesox on $node ... ${RESTORE}"
	echo $command
	ssh $user@$node $command
done

