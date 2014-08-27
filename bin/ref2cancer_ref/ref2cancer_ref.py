#!/usr/bin/env python
import sys
from Bio import SeqIO
from Bio.Seq import Seq
from Bio.Alphabet import generic_dna
from Bio.SeqRecord import SeqRecord
from optparse import OptionParser
import random
 
parser = OptionParser()
parser.add_option('-o','--output', dest = "outputFile", help = "Name of the output file")
parser.add_option('-r','--reference', dest = "reference", help = "Name of the reference file")
parser.add_option('-e','--rate', dest = "rate", help = "the rate of mutate to cancer  e/10000")
parser.add_option('-i','--id', dest = "id", help = "cancer id")
parser.add_option('-d','--desc', dest = "desc", help = "cancer description")


(options, args) = parser.parse_args(sys.argv[1:])
outFilename = options.outputFile
refFilename = options.reference
rate = int(options.rate)
cancerid = options.id
cancerdesc = options.desc

cancer_file_handle = open(outFilename, 'w')
vcf_file = open(outFilename+".vcf", 'w')
vcf_file.write("POS\tREF\tALT\tINFO\n")

for seq_record in SeqIO.parse(refFilename,"fasta"):
    cancer_list = list(seq_record.seq[:])
    cur_max = len(seq_record)-1
    for cur in range(0, cur_max):
	turn = random.randint(0,10000)
	if(turn < rate):
  	    r = random.randint(0,3)
 	    if(r==0):
		base = 'A'
	    elif(r==1):
		base = 'T'
	    elif(r==2):
		base = 'G'
  	    elif(r==3):
		base = 'C'
	    else:
		base = 'N'

	    if(base != cancer_list[cur]):
		vcf_file.write(str(cur)+"\t"+cancer_list[cur]+"\t"+base+"\t"+cancerid+" "+cancerdesc+"\n")
	        cancer_list[cur] = base
	    
    cancer_str = ''.join(cancer_list)
    cancer_record = SeqRecord(Seq(cancer_str, generic_dna), id= cancerid, description=cancerdesc)
    SeqIO.write(cancer_record, cancer_file_handle, 'fasta')

cancer_file_handle.close()

