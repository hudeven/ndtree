#!/bin/sh
gdb --args ndTree -idxfile ../data/index_real -dscdim 16 -load_file ../data/kmer_list -bqfile ../data/query100_box1 -skip 0 -newtree 1
