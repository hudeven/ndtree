#!/usr/bin/env python
import sys
from Bio import SeqIO
from optparse import OptionParser
import random

def letter2num(c):
    if (not cmp(c,'A')):
	elem = 0
    elif (not cmp(c,'T')):
	elem = 1
    elif (not cmp(c,'G')):
	elem = 2
    elif (not cmp(c,'C')):
	elem = 3
    else:
	elem = 4

    return elem


parser = OptionParser()
parser.add_option('-o','--output', dest = "outputFile", help = "Name of the output file")
parser.add_option('-k','--klength', dest = "kmerLength", help = "Length of kmer")
parser.add_option('-f','--readsfile', dest = "readsFile", help = "Name of the reads file")
parser.add_option('-n','--querynumber', dest = "querynumber", help = "number of random box query")
parser.add_option('-b','--boxsize', dest = "boxsize", help = "box size of random query")

(options, args) = parser.parse_args(sys.argv[1:])
outputFile = options.outputFile
readsFile = options.readsFile
kmerLength = int(options.kmerLength)

querynum = int(options.querynumber)
boxsize = int(options.boxsize)

query_file = open(outputFile, 'w')
cur = 0
cur_query = 0

for seq_record in SeqIO.parse(readsFile,"fasta"):
    cur_max = len(seq_record) - kmerLength
    #print(seq_record.id)
    #kmer_file.write(seq_record.id)
    for k in range(0, querynum):
        #cur = random.randint(0, len(seq_record)- kmerLength -1)
	cur = k
	box_loc = random.randint(0, kmerLength-1)
        query = ''
   	line = ''
	for i in range(0, kmerLength):
	    if(i == box_loc):
		curboxsize = boxsize
	    else:
		curboxsize = 1
	   
	    c = str(seq_record.seq[cur + i])
	    line = str(curboxsize) + ' ' + str(letter2num(c))  
	    curboxsize -= 1
	    elem = -1
	    while(curboxsize > 0):
	        elem += 1
		if(elem == letter2num(c)):
		    continue
		
		line += ' ' + str(elem)
		curboxsize -= 1
	
	    query = query + line + '\n'
	
	query_file.write(query)       
	cur = cur + 1

query_file.close()

#print(repr(seq_record.seq))
#print(len(seq_record))
#record_iterator = SeqIO.parse("16s.fa", "fasta")
#record = SeqIO.read90
#first = next(record_iterator)	
#while first is not None:
#    print(first.id)
#    print(first.description)
#    first = next(record_iterator)
