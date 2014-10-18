source nesox.sh

command=startnesox.sh

for node in $nodes
do
	echo "start nesox on $node"
	echo $command
	ssh $user@$node $command
done

