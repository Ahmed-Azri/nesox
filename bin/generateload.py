#!/usr/bin/python

import random
import sys
import decimal

def usage():
	print "usage: generateload.py [mean (megabytes)] [variance (megabytes)] [delay (seconds)]"

kilobytes = 1024
megabytes = 1048576
gigabytes = 1073741824

pairs = []
argc = len(sys.argv)
argv = sys.argv

mean = megabytes
variance = 0
delay = 5


if (argc >= 5):
	usage()
	quit()

if (argc == 2):
	mean = megabytes * float(argv[1])
if (argc == 3):
	mean = megabytes * float(argv[1])
	variance = megabytes * float(argv[2])
if (argc == 4):
	mean = megabytes * float(argv[1])
	variance = megabytes * float(argv[2])
	delay = int(argv[3])


for s in range(1,5):
	for d in range(1,5):
		pairs.append((s,d))

random.shuffle(pairs)


for pair in pairs:
	rand = long(round(random.gauss(mean, variance)))
	if rand < 0: rand = 0
	print '{0} {1} {2} {3}'.format(pair[0], pair[1], rand, delay)

