source nesox.sh

if [ "$1" = "" ]
then
command="rm -rf $logs/*.log"
else
command="rm -rf $logs/nesox-reader-*.log"
fi

for node in $nodes
do
	echo -e "${BLUE}clean nesox logs on $node ... ${RESTORE}"
	echo $command
	ssh $user@$node $command
done
