os=`uname`
bin=`dirname "$0"`
home=`cd "$bin"/../; pwd`
logs=`cd "$home"/logs; pwd`
host=127.0.0.1
user=root

if [ "$os" = "Linux" ]; then host=`hostname -i`; fi
file=$home/data/biblewarpease
inputnodes="$bin"/nodes
inputports="$bin"/ports

nodes=
ports=

thousand=000
million=000000

while read node
do
	nodes="$nodes $node"
done < $inputnodes

while read port
do
	ports="$ports $port"
done < $inputports

info()
{
	echo "OS:" $os
	echo "Nesox Home:" $home
	echo "File:" $file
	echo "Hostname:" $host
	echo "Nodes:" $nodes
	echo "Ports:" $ports
}

source color

