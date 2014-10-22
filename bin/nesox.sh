os=`uname`
bin=`dirname "$0"`
bin=`cd $bin; pwd`
home=`cd "$bin"/../; pwd`
logs=`cd "$home"/logs; pwd`
stat=`cd "$home"/stat; pwd`
load=`cd "$home"/load; pwd`
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

nodeprefix="202.45.128.17"
portprefix="817"

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

# Usage: strstr s1 s2
#
# Strstr echoes a substring starting at the first occurrence of string s2 in
# string s1, or nothing if s2 does not occur in the string.  If s2 points to
# a string of zero length, strstr echoes s1.
function strstr ()
{
    # if s2 points to a string of zero length, strstr echoes s1
    [ ${#2} -eq 0 ] && { echo "$1" ; return 0; }

    # strstr echoes nothing if s2 does not occur in s1
    case "$1" in
    *$2*) ;;
    *) return 1;;
    esac

    # use the pattern matching code to strip off the match and everything
    # following it
    first=${1/$2*/}

    # then strip off the first unmatched portion of the string
    echo "${1##$first}"
}

function getloadname()
{
	read loadpathname < $bin/status
	loadpathnamelength=${#loadpathname}

	loadfilename=`strstr $loadpathname "load"`
	loadfilenamelength=${#loadfilename}


	loadpathnamelength=$(expr $loadpathnamelength - 3)
	loadfilenamelength=$(expr $loadfilenamelength - 8)

	if [ "$loadfilename" = "" ]
	then
		loadname=${loadpathname:0:loadpathnamelength}
	else
		loadname=${loadfilename:5:loadfilenamelength}
	fi

	echo $loadname
}

source color
