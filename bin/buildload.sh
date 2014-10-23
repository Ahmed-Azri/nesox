source nesox.sh


function generateone()
{

mean="$1"
variance="$2"
delay="$3"
algorithm="$4-"
if [ "$4" = "" ]; then algorithm="hp-"; fi
timeinstant=`timestamp`
loadname="norm-$mean-$variance-$algorithm$timeinstant"
loadfile="$load/$loadname.ld"

generateload.py $mean $variance $delay > $loadfile

}

# can generate from 100 to 900 for drawing a line
#
function a2a()
{

generateone 100 0
generateone 200 0
generateone 400 0
generateone 800 0

}

a2a


