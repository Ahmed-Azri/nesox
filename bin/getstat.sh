source nesox.sh

remotehome=/root/nesox
option=$1

if [ "$option" = "2" ]
then
scp -r root@d0:$remotehome/stat2/* $home/stat2/
else
scp -r root@d0:$remotehome/stat1/* $home/stat1/
fi
