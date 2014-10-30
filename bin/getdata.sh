source nesox.sh

remotehome=/root/nesox

echo "get data files from nesox master node"
scp -r root@d0:$remotehome/stat/*.data $home/archive/

