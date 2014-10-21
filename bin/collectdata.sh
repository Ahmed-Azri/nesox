source nesox.sh

for node in $nodes
do
	echo -e "${RED}collect nesox data from $node ... ${RESTORE}"
	scp -r $user@$node:$logs $home/stat/"$node-logs"
done
