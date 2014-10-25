source nesox.sh

cd $load

loadfiles=*.ld
towait=30

for loadfile in $loadfiles
do
echo -e "${YELLOW}process load: $loadfile${RESTORE}"
loadbench.sh -f $loadfile
echo -e "please wait for $towait second(s)"
sleep $towait
done
