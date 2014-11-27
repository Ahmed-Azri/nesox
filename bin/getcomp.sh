source nesox.sh

remotehome=/root/nesox

echo "get stat files from nesox master node"
scp -r root@d0:$remotehome/stat/*.com* $home/archive/

