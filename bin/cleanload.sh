source nesox.sh

rm -rf $home/stat/*

command="rm -rf $home/loadlogs/*"
$command

if [ "$os" != Linux ]; then exit 0; fi;

for node in $nodes
do
	echo -e "${BLUE}clean nesox logs on $node ... ${RESTORE}"
	echo $command
	ssh $user@$node $command
done
