#!/usr/bin/env python
import sys
from Bio import pairwise2
from Bio.SubsMat import MatrixInfo as matlist
from Bio import SeqIO
from optparse import OptionParser

def letter2num(c):
     if (not cmp(c,'A')):
         elem = '0'
     elif (not cmp(c,'T')):
         elem = '1'
     elif (not cmp(c,'G')):
         elem = '2'
     elif (not cmp(c,'C')):
         elem = '3'
     else:
         elem = '4'
 
     return elem

parser = OptionParser()
parser.add_option('-k', '--klength', dest = 'klength', help = "the length of kmer")
parser.add_option('-o', '--output', dest = 'output', help = "the filename of box query file")
parser.add_option('-r', '--reference', dest = 'reference', help = " the filename of reference sequence")
parser.add_option('-s', '--sample', dest = 'sample', help = 'the filename of sample sequence')

(options, args) = parser.parse_args(sys.argv[1:])
klength = int(options.klength)
ref_filename = options.reference
sample_filename = options.sample
query_filename = options.output
query_letter_filename = options.output+".letter"

matrix = matlist.blosum62
gap_open = -10
gap_extend = -0.5

query_letter_file = open(query_letter_filename, 'w')
query_file = open(query_filename, 'w')
record1 = SeqIO.read(open(ref_filename),"fasta")
record2 = SeqIO.read(open(sample_filename),"fasta")
ref = str(record1.seq[:])
sample = str(record2.seq[:])

#p53_human = "MEEPQSDPSVEPPLSQETFSDLWKLLPENNVLSPLPSQAMDDLMLSPDDIEQWFTEDPGP"
#p53_mouse = "MEESQSDISLELPLSQETFSGLWKLLPPEDILPSPHCMDDLLLPQDVEEFFEGPSEALRV" 
alns = pairwise2.align.globalds(ref, sample, matrix, gap_open, gap_extend)  
top_aln = alns[0]
aln_ref, aln_sample, score, begin, end = top_aln
end = len(aln_ref)
for i in range(0, end):
    if(aln_ref[i] != aln_sample[i]):
	query_letter = '';
	query = '';
	if( i + klength >= end):
	    kmer = aln_ref[end-1-klength : end-1]
	    for p in range(end-1-klength, end):
		if(p == i):
		    query_letter += '1' + ' '  + aln_sample[p]+'\n' 
		    query += '1' + ' ' + letter2num(aln_sample[p])+'\n' 
		else:
		    query_letter += '1' + ' ' + aln_ref[p] + '\n'
		    query += '1' + ' ' + letter2num(aln_ref[p]) + '\n'
	else:
	    kmer = aln_ref[i : i+klength]   
	    for p in range(i, i+klength):
		if(p == i):
		    query_letter += '1' + ' ' + aln_sample[p]+'\n' 
		    query += '1' + ' ' + letter2num(aln_sample[p])+'\n' 
		else:
		    query_letter += '1' + ' ' + aln_ref[p] + '\n'
		    query += '1' + ' ' + letter2num(aln_ref[p]) + '\n'

		
	query_letter_file.write(str(i)+"\t"+kmer+'\n'+query_letter)
	query_file.write(query)
	print str(i)+"\t"+kmer+"\n"
	
query_file.close()
query_letter_file.close();


#print aln_ref+'\n'+aln_sample

