source nesox.sh

applications=$sched/maclearner.py
mainappname=maclearner
logfilename=$mainappname
logpathname=$logs


echo -e "${GREEN}start nesox ryu applications: $applications ${RESTORE}"
ryu-manager --log-dir $logpathname --log-file $logfilename $applications

