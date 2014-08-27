#!/bin/sh
gdb --args ndTree -idxfile ../data/index_real -dscdim 16 -load_file ../data/cancer/kmer/all.kmer -bqfile ../data/cancer/query/boxquery -skip 0 -newtree 1 -aux_file ../data/cancer/kmer/all.kmer.desc -record_file ../data/record
