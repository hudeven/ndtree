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
parser.add_option('-f','--readsfile', dest = "readsFilename", help = "Name of the reads file")
parser.add_option('-c','--coverage', dest = "coverage", help = "coverage for sequencer")
parser.add_option('-e','--error', dest = "error", help = "error rate for sequencer e/1000")


(options, args) = parser.parse_args(sys.argv[1:])
kmerFilename = options.outputFile
readsFilename = options.readsFilename
kmerLength = int(options.kmerLength)
coverage = int(options.coverage)
error = int(options.error)

kmer_file = open(kmerFilename, 'w')
for lp in range(coverage):
    for seq_record in SeqIO.parse(readsFilename,"fasta"):
	cur = 0
        cur_max = len(seq_record) - kmerLength
        #print(seq_record.id)
        #kmer_file.write(seq_record.id)
        while cur <= cur_max:
	    kmer = list(seq_record.seq[cur:cur+kmerLength]);
	    i = 0	
            for mer in kmer:
	        c = letter2num(mer)
	        tmp = random.randint(1,1000)
		if(tmp <= error):
		    fakeC = random.randint(0,3)
		    while(c == fakeC):
    		        fakeC = random.randint(0,3)
 
	            kmer[i] = str(fakeC)
	        else:
		    kmer[i] = str(c)
   	
	        i = i+1

	    kmerStr = ' '.join(kmer)
	    kmer_file.write(kmerStr + "\n")       
	    cur = cur + 1

kmer_file.close()

#print(repr(seq_record.seq))
#print(len(seq_record))
#record_iterator = SeqIO.parse("16s.fa", "fasta")
#record = SeqIO.read90
#first = next(record_iterator)	
#while first is not None:
#    print(first.id)
#    print(first.description)
#    first = next(record_iterator)
