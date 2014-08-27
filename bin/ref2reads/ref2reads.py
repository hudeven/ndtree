#!/usr/bin/env python
import sys
from Bio import SeqIO
from optparse import OptionParser
import random
 
parser = OptionParser()
parser.add_option('-o','--output', dest = "outputFile", help = "Name of the output file")
parser.add_option('-l','--readlength', dest = "readlength", help = "lengh of read")
parser.add_option('-r','--reference', dest = "reference", help = "Name of the reference file")
parser.add_option('-c','--coverage', dest = "coverage", help = "coverage for sequencer")
parser.add_option('-e','--error', dest = "error", help = "error rate for sequencer e/1000")
parser.add_option('-t','--type', dest = "error", help = "error rate for sequencer e/1000")


(options, args) = parser.parse_args(sys.argv[1:])
outFilename = options.outputFile
refFilename = options.reference
readlength = int(options.readlength)
coverage = int(options.coverage)
error = int(options.error)



#There should be one and only one record, the entire genome:
record = SeqIO.read(open(refFilename), "fasta")
 
frags=[]
limit=len(record.seq)
readnum = coverage * limit / readlength
for i in range(0, readnum) :
    start=random.randint(0,limit-readlength)
    end=start+readlength
    frag=record.seq[start:end]
    readitem=SeqIO.SeqRecord(frag,id= str(i+1000),name=record.name, description=record.description)
    frags.append(readitem)
 
output_handle = open(outFilename, "w")
SeqIO.write(frags, output_handle, "fasta")
output_handle.close()
