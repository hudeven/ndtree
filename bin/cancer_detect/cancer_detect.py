import os
import sys
from optparse import OptionParser

#Modify it to your working dir
WORKING_DIR = "/home/steven/workspace/ndtree/"

parser = OptionParser()
parser.add_option('-n','--count', dest = "count", help = "number of random cancer dna")
parser.add_option('-r','--repeat', dest = "repeat", help = "repeat times")
parser.add_option('-k','--klength', dest = "klength", help = "lenght of kmer")
(options, args) = parser.parse_args(sys.argv[1:])

count = int(options.count)
klength = options.klength
repeat = int(options.repeat)
for i in range(0, repeat):
	print "******generate cancers' sequences from reference******\n"
	os.chdir(WORKING_DIR + "bin/ref2cancer_ref/")
	for i in range(0, count):
	    cmd = "python ref2cancer_ref.py --output ../../data/cancer/ref/"+str(i)+"_ref.fa --reference ../../data/test.fa --id "+ str(i) + " --desc "+ "cancer"+str(i)+" --rate 100"
	    os.system(cmd)

	#os.chdir(WORKING_DIR + "bin/ref2reads/")
	#for i in range(0, count):
	#    cmd = "python ref2reads.py --output ../../data/cancer/read/" + str(i) + "_reads.fa --readlength 100 --reference ../../data/cancer/ref/" + str(i) + "_ref.fa --coverage 20 --error 0"
	 #   os.system(cmd)

	print "******generate kmers from reads******\n"
	os.chdir(WORKING_DIR + "bin/reads2kmer/")
	for i in range(0, count):
	    cmd = "python fasta_kmer.py --output ../../data/cancer/kmer/" + str(i)+".kmer --klength "+ klength  +" --readsfile ../../data/cancer/ref/" + str(i) + "_ref.fa"
	    os.system(cmd)
	print "****** combine all cancers' kmers to one file ******\n"
	os.chdir(WORKING_DIR + "data/cancer/kmer/")
	cmd1 = "cat "
	cmd2 = "cat "
	cmd3 = "cat "
	cmd4 = "cat "
	for i in range(0, count):
	    cmd1 += str(i)+".kmer "
	    cmd2 += str(i)+".kmer.id "
	    cmd3 += str(i)+".kmer.desc "
	    cmd4 += str(i)+".kmer.name "
	cmd1 += "> all.kmer"
	cmd2 += "> all.kmer.id"
	cmd3 += "> all.kmer.desc"
	cmd4 += "> all.kmer.name"
	os.system(cmd1)
	os.system(cmd2)
	os.system(cmd3)
	os.system(cmd4)
	 
	print "******align sample sequence with respect to reference******\n"
	os.chdir(WORKING_DIR + "bin/sample2boxquery/")
	#cmd = 'python sample2boxquery.py --klength 16 --output /home/steven/workspace/ndtree/data/cancer/query/boxquery --reference /home/steven/workspace/ndtree/data/test.fa --sample /home/steven/workspace/ndtree/data/cancer/ref/0_ref.fa'

	#fast one: no alignment, just compare
	cmd = 'python sample2boxquery_simple.py --klength '+klength +' --output /home/steven/workspace/ndtree/data/cancer/query/boxquery --reference /home/steven/workspace/ndtree/data/test.fa --sample /home/steven/workspace/ndtree/data/cancer/ref/0_ref.fa'
	os.system(cmd)
	
	print "******run bond-tree ******\n"
	os.chdir(WORKING_DIR + "src/")
	cmd = './ndTree -idxfile ../data/index_real -dscdim '+ klength  +' -load_file '+ '../data/cancer/kmer/all.kmer'+ ' -bqfile ../data/cancer/query/boxquery -skip 0 -newtree 1 -aux_file ../data/cancer/kmer/all.kmer.desc -record_file ../data/record'
	os.system(cmd)

	print "*****cancer type sets intersection ******\n"
	os.chdir(WORKING_DIR + "bin/intersect_result")
	cmd = 'python intersect_result.py --input /home/steven/workspace/ndtree/data/cancer/query/boxquery.result --output  /home/steven/workspace/ndtree/data/cancer/result/cancer_type'
	os.system(cmd)
