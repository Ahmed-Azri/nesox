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


reference
---------
http://bashcookbook.com/bashinfo/source/bash-3.1/examples/scripts.noah/string.bash
