source nesox.sh

readerlogfiles=nesox-reader*.log
serverlogfiles=nesox-server*.log

if [ "$1" = "" ]
then
aggregatefile="$stat/$1"
else
aggregatefile="$stat/stat.data"
fi

for readerlog in $readerlogfiles
do
	s=${readerlog:31:1}
	d=${readerlog:26:1}
	printf "(%s):%s->%s" $readerlog $s $d
	echo
done
