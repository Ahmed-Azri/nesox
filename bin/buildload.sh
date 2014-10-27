source nesox.sh

function generateone()
{

mean="$1"
variance="$2"
delay="$3"
algorithm="$4-"
if [ "$4" = "" ]; then algorithm=""; fi
timeinstant=`timestamp`
loadname="norm-$variance-$mean-$algorithm$timeinstant"
loadfile="$load/$loadname.ld"

generateload.py $mean $variance $delay > $loadfile

}


# all-to-all communication: from 100 megabytes to 1000 megebytes
#
function a2a()
{
for mean in $(seq -w 100 100 1600)
do
	generateone $mean 0000
done
}


# shuffle communication: some flow will be zero
#
function shuffle()
{
for mean in $(seq -w 100 100 1600)
do
	generateone $mean $mean
done
}

a2a
shuffle
