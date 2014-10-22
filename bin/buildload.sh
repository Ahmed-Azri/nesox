source nesox.sh


function generateone()
{

mean="$1"
variance="$2"
delay="$3"
loadname="norm-$mean-$variance"
loadfile="$load/$loadname.ld"

generateload.py $mean $variance $delay > $loadfile

}

function a2a()
{

generateone 100 0
generateone 200 0
generateone 400 0
generateone 800 0

}

a2a


