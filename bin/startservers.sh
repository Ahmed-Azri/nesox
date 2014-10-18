#!/bin/bash

OS=`uname`
FILE=$NESOX_HOME/data/biblewarpease

if [ $OS = Linux ] 
then HOST=`hostname -i`;
else HOST=127.0.0.1; fi

echo "OS: "$OS
echo "Nesox Home: "$NESOX_HOME
echo "File: "$FILE
echo "Hostname: "$HOST
exit

nesox -r server -g background -f $FILE $HOST 8881
nesox -r server -g background -f $FILE $HOST 8882
nesox -r server -g background -f $FILE $HOST 8883
nesox -r server -g background -f $FILE $HOST 8884

