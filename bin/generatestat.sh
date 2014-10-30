source nesox.sh

makestat()
{

statname="$1"

f="0100"
s="0100"
l="1600"

for comparg in $(seq 2 3)
do
	nameposfix="com$comparg"
	outputname="$stat/$statname-$f-$l-$nameposfix.stat"

	for mean in $(seq -w $f $s $l)
	do
		if [ "$statname" = "all2all" ]; then variance="0000"; fi
		if [ "$statname" = "shuffle" ]; then variance="$mean"; fi
		nameprefix="norm-$variance-$mean"
		inputnames="$stat/$nameprefix*$nameposfix"

		x=$mean
		for inputname in $inputnames
		do
			read y < $inputname
			echo "$x $y" >> $outputname
		done
	done
done

}

makestat all2all
makestat shuffle
