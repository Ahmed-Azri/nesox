source nesox.sh

for port in $ports
do
	echo -e "${RED}test nesox on port $port ... ${RESTORE}"
	nesox -r reader -s 1024 $host $port
	echo -e "${GREEN}OK!!!${RESTORE}"
done
