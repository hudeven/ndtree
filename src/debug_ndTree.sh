#!/bin/sh
sudo gdb --args ndTree -idxfile ../data/index_real -dscdim 16 -load_file ../data/data_random -bqfile ../data/query_random -skip 0 -newtree 1 -aux_file ../data/cancer/kmer/all.kmer.desc -record_file ../data/record
