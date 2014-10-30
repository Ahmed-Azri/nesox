source nesox.sh

cd $load

loadfiles=*.ld
if [ "$1" != "" ]; then towait="$1"; else towait=30; fi
if [ "$2" != "" ]; then interactive="$2"; else interactive=""; fi


for loadfile in $loadfiles
do

if [ "$interactive" != "" ];
then
echo -e "${YELLOW}Before press any key to contunue: disable Openflow${RESTORE}"
read waitforinput
fi

echo -e "${YELLOW}process load: $loadfile${RESTORE}"
loadbench.sh -f $loadfile

echo -e "${YELLOW}Quickly enable Openflow Instance within 5 seconds!!!${RESTORE}"
echo -e "${YELLOW}please wait for $towait second(s)${RESTORE}"
sleep $towait

done
