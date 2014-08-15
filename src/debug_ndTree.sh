#!/bin/sh
gdb --args ndTree -idxfile ../data/index_real -dscdim 16 -load_file ../data/data_case0814 -bqfile ../data/query_case0814 -skip 0 -newtree 1
