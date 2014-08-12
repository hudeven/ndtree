#!/bin/sh
gdb --args ndTree -idxfile ../data/index_real -dscdim 16 -load_file ../data/data_random -bqfile ../data/query_random -skip 0 -newtree 1
