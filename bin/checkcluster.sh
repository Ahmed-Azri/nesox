source nesox.sh

command="ps -e -o pid -o time -o times -o cmd grep nesox"

for node in $nodes
do
	echo -e "${RED}check nesox on $node ... ${RESTORE}"
	echo $command
	ssh $user@$node $command
done
