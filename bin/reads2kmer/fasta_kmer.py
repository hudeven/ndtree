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
seqNameFile = kmerFilename + ".name"
seqIdFile = kmerFilename + ".id"
seqDescriptionFile = kmerFilename + ".desc"
readsFilename = options.readsFilename
kmerLength = int(options.kmerLength)

kmer_file = open(kmerFilename, 'w')
seqname_file = open(seqNameFile, 'w')
seqid_file = open(seqIdFile, "w")
seqdesc_file = open(seqDescriptionFile, 'w')

for seq_record in SeqIO.parse(readsFilename,"fasta"):
    cur = 0
    cur_max = len(seq_record) - kmerLength
    for cur in range(0, cur_max):
	kmer = list(seq_record.seq[cur:cur+kmerLength]);
	i = 0	
        for mer in kmer:
		if(cmp(mer, 'A')==0):
			kmer[i] = '0'
		elif(cmp(mer, 'T')==0):
			kmer[i] = '1'
		elif(cmp(mer, 'G')==0):
			kmer[i] = '2'
		elif(cmp(mer, 'C')==0):
			kmer[i] = '3'
		else:
			kmer[i] = '4'
		i = i+1
	kmerStr = ' '.join(kmer)
	kmer_file.write(kmerStr + "\n")       
	seqname_file.write(seq_record.name+"\n")
	seqid_file.write(seq_record.id+"\n")
	seqdesc_file.write("1000 "+seq_record.description+"\n");
kmer_file.close()

