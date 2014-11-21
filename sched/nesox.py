import platform as p

if p.system() == 'Linux':
	transdir = '/root/nesox/trans/'
if p.system() == 'Darwin':
	transdir = '../trans/'

class flow:
	def __init__(self, source, destination, datasize,
		stime = 0, etime = 0, interval = 0, findex = 0):
		self.findex = findex
		self.source = source
		self.destination = destination
		self.datasize = datasize
		self.stime = stime
		self.etime = etime
		self.interval = interval

	def show(self, mode = 0):
		if (mode == 0):
			print '{0} {1} {2}'.format(self.source, self.destination, self.datasize)
		if (mode != 0):
			print '{0} {1} {2} {3} {4} {5}'.format(self.source, self.destination, self.datasize,
				self.stime, self.etime, self.interval)

if __name__ == '__main__':
	f = flow(1,2,1024)
	f.show(1)

