source nesox.sh

command="rm -rf $beat/*.log"

for node in $nodes
do
	echo -e "${YELLOW}clean beater logs on $node ... ${RESTORE}"
	echo $command
	ssh $user@$node $command
done
