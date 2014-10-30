todo list
=========

**When a new idea comes out, do not miss it!**

Architecture
------------

standing (daemon) roles: master; workers/slaves (servers);
communication (connection) roles: client/server;

Names
-----
Nicky, Matthew, Carl, PK
Marco, Steve, PK

nesox system input and output
-----------------------------
**input: transfer matrix**
form: wordloads (loadN.ld)

**output:completion time matrix**
form: compute.c ->


statistics data file format
---------------------------
source:destination:data-size:start-time:end-time:time-interval


load distribution and generation
--------------------------------
what kind of transfer matrix is represative?
1. generate N random numbers of given distribution (normal) according to: mean and variance
2. put the N random numbers into the m*m (=N) matrix as workload


features of a transfer (a set of flows)
---------------------------------------
1. total data size:
2.   max data size:
3. total completion time:
4. average completion time per flow and variance:
5. transfer completion time: (latest - earliest)
6. first complete flow size
8. last complete flow size

naming convensions
------------------
loadname = norm-$mean-$variance | all-to-all-$datasize
loadfile = $loadname.ld
algorithm = pdq | ...
logaggregate = $loadname-$algorithm.data
statfile = $loadname-$algorithm.stat

e.g., norm-102400-100000.ld

nesox architecture
------------------
layers: OS - base lib (logger, daemon, message, ...) - protocal - nesox framework (master, server, client, ... ) - service providers (reader, loader, writer ... ) - applications
asides: ryu and nesox scheduler as ryu applications
asides: cluster admin scripts


controller plane
----------------
0. maclearner: done
1. how to goto table: OFPInstructionGotoTable (done)
2. how to match: OFPMatch (done: but source port does not work!!!)
3. how to modify table (flow): OFPFlowMod (done)
4. how to packetout: OFPPacketOut (done)
5. priority (scheduling): high priority match first (understood!!)
6. meter table operation: (done!!!)
7. raise the priority of a specific matching to force that matching happen!!!
8. to put code segments into the paper controller part!!!
9. how to create meter (done)
a. how to associate flow to meter (done)


reference
---------
http://bashcookbook.com/bashinfo/source/bash-3.1/examples/scripts.noah/string.bash
http://www.enderunix.org/docs/eng/daemon.php
