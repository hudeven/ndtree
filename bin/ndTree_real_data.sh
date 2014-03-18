#!/bin/sh
echo generate kmer list from real DNA sequence file
python reads2kmer/fasta_parser.py --output ../data/kmer_list --klength 16 --readsfile ../data/test1.fasta

echo run ndTree with kmer_list
../src/ndTree -idxfile ../data/index_real -dscdim 16 -load_file ../data/kmer_list -bqfile ../data/box_query_random -skip 0 -newtree 1
