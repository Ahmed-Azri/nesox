source nesox.sh

for port in $ports
do
	echo -e "${RED}start nesox on port $port ... ${RESTORE}"
	nesox -g background -r server -d $home -f $file $host $port
done
