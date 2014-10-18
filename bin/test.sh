#!/bin/bash

bin=`dirname "$0"`
bin=`cd "$bin"; pwd`
home=`cd "$bin"/../; pwd`

OS=`uname`
if [ $OS = Linux ]; then NESOX_HOME=/root/nesox; fi

FILE=$NESOX_HOME/data/biblewarpease

if [ $OS = Linux ] 
then HOST=`hostname -i`;
else HOST=127.0.0.1; fi

inputnodes="$bin"/nodes
inputports="$bin"/ports
nodes=
ports=

while read node
do
	nodes="$nodes $node"
done < $inputnodes

while read port
do
	ports="$ports $port"
done < $inputports

echo "OS: "$OS
echo "Nesox Home: "$NESOX_HOME
echo "File: "$FILE
echo "Hostname: "$HOST
echo "Nodes: "$nodes
echo "Ports: "$ports

for  node in $nodes
do 
	echo $node
done

exit

nesox -r server -g background -f $FILE $HOST 8881
nesox -r server -g background -f $FILE $HOST 8882
nesox -r server -g background -f $FILE $HOST 8883
nesox -r server -g background -f $FILE $HOST 8884

