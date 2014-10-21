source nesox.sh

command="ps -leaf | grep nesox"

for node in $nodes
do
	echo -e "${RED}check nesox on $node ... ${RESTORE}"
	echo $command
	ssh $user@$node $command
done

