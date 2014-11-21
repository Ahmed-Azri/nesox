from os import listdir
from nesox import flow
from nesox import transdir

flows = []
transfermap = {}
transfers = listdir(transdir)

for tran in transfers: transfile = open(transdir + tran)
for line in transfile: flows.append(flow(int(line[0]), int(line[2]), int(line[6:].rstrip()), int(line[4])))
for flow in flows: transfermap[(flow.source, flow.destination)] = flow

transfermap[(1,2)].show(1)

