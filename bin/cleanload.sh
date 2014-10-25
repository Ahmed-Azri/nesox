source nesox.sh

command="rm -rf $home/loadlogs/*.log"

for node in $nodes
do
	echo -e "${BLUE}clean nesox logs on $node ... ${RESTORE}"
	echo $command
	ssh $user@$node $command
done

rm -rf $home/stat/*
