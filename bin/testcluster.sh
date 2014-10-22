source nesox.sh

command=testnesox.sh

for node in $nodes
do
	echo -e "${BLUE}test nesox on $node ... ${RESTORE}"
	for port in $ports
	do
		nesox -r reader -s 1024 $node $port
	done
done
