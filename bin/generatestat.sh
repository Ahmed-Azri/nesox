source nesox.sh

makestat()
{

statname="$1"

for comparg in $(seq 2 3)
do
	nameposfix="com$comparg"
	outputname="$statname-$nameposfix.stat"

	for mean in $(seq -w 100 100 1600)
	do
		if [ "$statname" = "all2all" ]; then variance="0000"; fi
		if [ "$statname" = "shuffle" ]; then variance="$mean"; fi
		nameprefix="norm-$variance-$mean"
		inputnames="$nameprefix*$nameposfix"
		echo "$statname-$nameposfix" > $outputname

		x=$mean
		for inputname in $inputnames
			read y < $inputname
			echo "$x $y" >> $outputname
		done
	done
done

}

makestat all2all
makestat shuffle
