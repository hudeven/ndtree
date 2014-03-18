#!/bin/sh
gdb --args ../src/ndTree -idxfile ../data/index_real -dscdim 16 -load_file ../data/kmer_list -bqfile ../data/box_query_random -skip 0 -newtree 1
