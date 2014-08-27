#!/usr/bin/env python
import sys
from optparse import OptionParser

parser = OptionParser()
parser.add_option('-i', '--input', dest = 'inputFile', help = 'input file name')
parser.add_option('-o', '--output', dest ='outputFile', help = 'output file name')

(options, args) =parser.parse_args(sys.argv[1:])
inputFilename = options.inputFile
outputFilename = options.outputFile

first = True
with open(inputFilename,"r") as f:
    for line in f:
	word = line.split()
	if(first):
	    result = word[1:]
	    first = False
	    continue
	else:
	    w = word[1:]
	    result = set(result).intersection(w)

result_file = open(outputFilename, "a")
for r in result:
    print r
    result_file.write(r+"\t")
result_file.write("\n")
result_file.close()
	    
