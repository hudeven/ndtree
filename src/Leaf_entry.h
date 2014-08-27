#include "config.h"

#ifndef LEAF_ENTRY_H
#define LEAF_ENTRY_H

struct Leaf_entry
{
    unsigned char key[DIM]; // Key: a string of DIM characters, represented as 0 to (alphabet_size-1)
    float cntkey[CNTDIM]; // Continuous dimensions. A string of CNTDIM floating point values. 
    ND_tree_record_count record_count; // Actual data, usually a pointer
    ND_tree_record record;    
};

#endif
