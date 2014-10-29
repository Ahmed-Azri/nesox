source nesox.sh

applications="$sched/policyengine.py $sched/dispatcher.py $sched/scheduler.py $sched/rateallocator.py $sched/maclearner.py"
mainappname=controller.log
logfilename=$mainappname
logpathname=$logs


echo -e "${GREEN}start nesox ryu applications: $applications ${RESTORE}"
ryu-manager --log-dir $logpathname --log-file $logpathname/$logfilename $applications

