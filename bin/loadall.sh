source nesox.sh

cd $load

loadfiles=*.ld
if [ "$1" != "" ]; then towait="$1"; else towait=30; fi

echo -e "${YELLOW}please wait for $towait second(s)${RESTORE}"
sleep $towait

for loadfile in $loadfiles
do
echo -e "${YELLOW}process load: $loadfile${RESTORE}"
loadbench.sh -f $loadfile
done
