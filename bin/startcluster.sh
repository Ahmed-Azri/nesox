source nesox.sh

command=startnesox.sh

for node in $nodes
do
	echo "${RED}start nesox on $node${RESTORE}"
	echo $command
	ssh $user@$node $command
done
