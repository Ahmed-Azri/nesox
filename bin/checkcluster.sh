source nesox.sh

command="ps -e -o pid -o time -o cmd | grep -v grep | grep nesox"

for node in $nodes
do
	echo -e "${BLUE}check nesox on $node ... ${RESTORE}"
	echo $command
	ssh $user@$node $command
done
