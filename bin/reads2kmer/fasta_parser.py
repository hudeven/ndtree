#!/usr/bin/env python
import sys
from Bio import SeqIO
from optparse import OptionParser

parser = OptionParser()
parser.add_option('-o','--output', dest = "outputFile", help = "Name of the output file")
parser.add_option('-k','--klength', dest = "kmerLength", help = "Length of kmer")
parser.add_option('-f','--readsfile', dest = "readsFilename", help = "Name of the reads file")
(options, args) = parser.parse_args(sys.argv[1:])
kmerFilename = options.outputFile
readsFilename = options.readsFilename
kmerLength = int(options.kmerLength)

kmer_file = open(kmerFilename, 'w')
cur = 0
for seq_record in SeqIO.parse(readsFilename,"fasta"):
    cur_max = len(seq_record) - kmerLength
    #print(seq_record.id)
    #kmer_file.write(seq_record.id)
    while cur <= cur_max:
        kmer_file.write(str(seq_record.seq[cur:cur+kmerLength]) + "\n")       
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
