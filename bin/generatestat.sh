source nesox.sh

makestat()
{

statname="$1"

for comparg in $(seq 2 3)
do
	nameposfix="com$comparg"
	outputname="$stat/$statname-$nameposfix.stat"

	echo "$statname-$nameposfix" > $outputname
	for mean in $(seq -w 100 100 1600)
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
