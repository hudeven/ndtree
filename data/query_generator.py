#!/usr/bin/env python
import sys
from optparse import OptionParser
import random

parser = OptionParser()
parser.add_option('-o', '--output', dest="outputFile",\
    help="Name of the output file")
parser.add_option('-a', '--alphabet', dest="alpha",\
    help="Alphabet size")
parser.add_option('-n', '--count', dest="count",\
    help="Number of records to generate", default="1000")
parser.add_option('-d', '--dimension', dest="dim",\
    help="Number of dimension", default="16")


(options, args) = parser.parse_args(sys.argv[1:])

opFile = options.outputFile
alpha = int(options.alpha)
dim = int(options.dim)
count = int(options.count)
fd = open(opFile, "w")
# Generate box query files for box sizes 1 to 5
master = range(0, alpha-1)
for q in range(count):
	for i in range(dim):
		boxsize = random.randint(1, alpha-1)
		query = random.sample(master, boxsize)
		queryStr = ' '.join(map(str, query))
		fd.write( str(boxsize) + " " + queryStr + "\n")
fd.close()
