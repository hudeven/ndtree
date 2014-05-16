#ifndef NODE_H
#define NODE_H
#include "logClass.h"
#include "config.h"
#include "utility.h"

const int MAX_ALPHABET_SIZE = 256; // Define the maximum length of the ND-tree file name
const int MAX_DMBR_DIM_SIZE = (MAX_ALPHABET_SIZE - 1) / BITS_PER_BYTE + 1; // in bytes, max bitmap size for one dimension of DMBR
extern logClass logO;

//extern const int DIR_NODE_OVERHEAD;

//extern struct Dir_entry_copy{
//   unsigned char DMBR[DMBR_SIZE]; // Digital Minimum Bounding Rectangle
//   unsigned int child; // Block number of the child of the current entry
//};

class Node
{
#ifndef CONFIG_H
#define CONFIG_H

#include <stack>

// This file defines all the configuration parameters for the ND Tree program

enum ENVIRONMENT {UNIX, WINDOWS};
ENVIRONMENT RUNNING_ENVIRONMENT = UNIX;

const int QUERY_RESULTS_BUFFER_SIZE = 1000;

typedef int ND_tree_record;
const int MAX_LINE_IN_SOURCE_FILE = 1000000;

int DIM = 16;
const int CNTDIM = 0; 

//rest 2 are used as part of sourceData file name: sourceData32+10
const int TOTAL_DSC_VALUE =8; //total DSC values to be read out
const int TOTAL_CNT_VALUE =8;//total CNT values to be read out
const int TOTAL_BOX_QUERY_NUM = 10;
const int TOTAL_RANGE_QUERY_NUM =200;


const int ALPHA=4;
int A[] = {ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA
, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA
, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA
, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA
, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA
, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA
, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA
, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA}; 
const int BYTES_PER_DIM_IN_DMBR = ((ALPHA%8)==0)?(ALPHA/8):(ALPHA/8+1);

const int BOX_SIZE_START_AT = 1;
const int BOX_SIZE_STEP = 1;
const int BOX_SIZE_STOP_BEFORE = A[0];
const int RANGE_SIZE_STEP = 1;
const int RANGE_STOP_BEFORE = (DIM + CNTDIM)<10?(DIM + CNTDIM):10;

const int DISK_BLOCK_SIZE = 4096; 
const int DMBR_SIZE = DIM * BYTES_PER_DIM_IN_DMBR; 

int boxSize;

const int usedBytesForEntryHeader = ((DIM%8)==0)?(DIM/8):(DIM/8+1);;
//const int USE_LINK_FOR_BOX_QUERY=1;//don't change, set to 1 in all conditions
//const int USE_LINK_FOR_RANGE_QUERY=1;//don't change, set to 1 in all conditions
		
const int MAX_DIM_AND_CONTDIM = 32; //jan 09, maximum lines for cnt and dsc in boxqueryall file

enum SPLIT_TYPE { ORIGINAL, TO_DEATH/*do not use*/,TO_DEATH_MAX_UTIL/*do not use*/,TO_DEATH_RANDOM_UTIL };

// This options decides whether we use NDTree or BoNDTree
SPLIT_TYPE nodeSplitType  = ORIGINAL;//TO_DEATH_RANDOM_UTIL; 

enum ENUM_TREE_TYPE {STATIC_TREE, DYNAMIC_TREE};ENUM_TREE_TYPE TREE_TYPE = STATIC_TREE;

bool RESORT_2_OLD_SPLIT = true;//if true, might resort to old split when nodeSplitType is TO_DEATH_RANDOM_UTIL

//#define LOG_MBR_SPLITTING_LEAF
//#define LOG_MBR_SPLITTING_DIR

//#define LOG_EDGE_LENGTH_LEAF
//#define LOG_EDGE_LENGTH_DIR

//#define LOG_OLD_AND_NEW_BLOCK_LEAF

////#define LOG_UTILIZATION

//#define LOG_ENTRY_ON_SPLITTING_DIM 
//#define ShorterThanSplitDim

//#define LOG_KANPSACK_VALUE_AND_WEIGHT

int heuristics_overlap_used_leaf =0;
int heuristics_area_used_leaf =0;
int heuristics_overlap_used_dir =0;
int heuristics_area_used_dir =0;
int BestChild_covered_area =0;
int BestChild_notcovered_overlap_enlarge =0;
int BestChild_notcovered_area =0;
int BestChild_notcovered_area_enlarge =0;

const int enforce_minUtil_for_exhaustSplit =1;//always 1
const int try_all_dim_with_minUtil = 1;



#endif
public:
    Node(int* alphabet_sizes);
    ~Node();

    //virtual void read_node(fstream& ND_tree_file, unsigned int block_number) = 0;
    //virtual void write_node(fstream& ND_tree_file, unsigned int block_number) = 0;
    void log_DMBR(const unsigned char* const DMBR);
    void log_OneDirEntry_OnOneDscDim(  unsigned char*  DMBR, int dimNum,int* alphabet_sizes ); 
protected:
    double cal_normed_area(unsigned char* DMBR, int* alphabet_sizes); // Calculate the normalized area of a DMBR
    double cal_normed_overlap(unsigned char* DMBR1, unsigned char* DMBR2, int* alphabet_sizes); // Calculate the normalized overlap area of DMBR1 and DMBR2
    bool set_equal(unsigned char* set1, unsigned char* set2, int byte_size);
    void set_intersect(unsigned char* set1, unsigned char* set2, int byte_size, unsigned char* intersect_set, bool& is_intersect); // Return the intersect of two sets
    void bitmap_to_letters(unsigned char* bitmap_set, int byte_size, unsigned char* letters, int &letter_count); // Given a bitmap, generate a corresponding letter list
    int set_size(unsigned char* bitmap_set, int byte_size);


////Steven: change to dynamic allocation

    // Auxiliary data
    int* DMBR_byte_lut[MAX_ALPHABET_SIZE]; //// [DIM][MAX_ALPHABET_SIZE]; // Given a dim and a letter, store the coresponding byte in DMBR
    int* DMBR_bit_lut[MAX_ALPHABET_SIZE]; //// [DIM][MAX_ALPHABET_SIZE]; // Given a dim and a letter, store the coresponding bit in DMBR
    int* DMBR_start_byte_lut;////[DIM]; // Given a dim, store the coresponding start byte in DMBR
    int* DMBR_end_byte_lut; //// [DIM]; // Given a dim, store the coresponding end byte in DMBR
   
////Steven

    int* bitmap_byte_lut; // Given a letter, store the coresponding byte in a bitmap
    int* bitmap_bit_lut; // Given a letter, store the coresponding bit in a bitmap
 
	//int getCompressedDiskSize(vector<Dir_entry_copy> & tmp_entries, int *alphabet_sizes);

};

Node::~Node()
{

    delete [] DMBR_start_byte_lut;
    delete [] DMBR_end_byte_lut;
    delete [] bitmap_byte_lut;
    delete [] bitmap_bit_lut;

   for(int i=0; i<DIM; i++)
   {
       delete [] DMBR_byte_lut[i];
       delete [] DMBR_bit_lut[i];

    }
 


}
Node::Node(int* alphabet_sizes)
{
    //// Steven:
    
    for(int i=0; i<DIM; i++)
    {
       DMBR_byte_lut[i] = new int[MAX_ALPHABET_SIZE];//// [DIM][MAX_ALPHABET_SIZE]; // Given a dim and a letter, store the coresponding byte in DMBR
       DMBR_bit_lut[i] = new int[MAX_ALPHABET_SIZE]; //// [DIM][MAX_ALPHABET_SIZE]; // Given a dim and a letter, store the coresponding bit in DMBR
    }

    DMBR_start_byte_lut = new int[DIM];////[DIM]; // Given a dim, store the coresponding start byte in DMBR
    DMBR_end_byte_lut = new int[DIM]; //// [DIM]; // Given a dim, store the coresponding end byte in DMBR
    bitmap_byte_lut = new int[MAX_ALPHABET_SIZE];
    bitmap_bit_lut = new int[MAX_ALPHABET_SIZE];

    // compute auxiliary table
    int tmp_byte = 0;
    int i, j;
    for(i = 0; i < DIM; i++)
    {
        DMBR_start_byte_lut[i] = tmp_byte;
        for(j = 0; j < alphabet_sizes[i]; j++)
        {
            DMBR_byte_lut[i][j] = tmp_byte + j / BITS_PER_BYTE;
            DMBR_bit_lut[i][j] = j % BITS_PER_BYTE;
        }
        tmp_byte += (alphabet_sizes[i] - 1) / BITS_PER_BYTE + 1;
        DMBR_end_byte_lut[i] = tmp_byte - 1;
    }
    for(i = 0; i < MAX_ALPHABET_SIZE; i++)
    {
        bitmap_byte_lut[i] = i / BITS_PER_BYTE; 
        bitmap_bit_lut[i] = i % BITS_PER_BYTE; 
    }
}

double Node::cal_normed_area(unsigned char* DMBR, int* alphabet_sizes)
{
    int i, j;
    int tmp_sum;
    double area = 1.0;
    for(i = 0; i < DIM; i++)
    {
        tmp_sum = 0;
        for(j = DMBR_start_byte_lut[i]; j <= DMBR_end_byte_lut[i]; j++)
            tmp_sum += bit_1_count_lut[DMBR[j]];
        if(tmp_sum == 0)
            return 0.0;
        area *= static_cast<double>(tmp_sum) / alphabet_sizes[i];
    }
    return area;
}

double Node::cal_normed_overlap(unsigned char* DMBR1, unsigned char* DMBR2, int* alphabet_sizes)
{
    int i, j;
    int tmp_sum;
    double overlap = 1.0;
    for(i = 0; i < DIM; i++)
    {
        tmp_sum = 0;
        for(j = DMBR_start_byte_lut[i]; j <= DMBR_end_byte_lut[i]; j++)
            tmp_sum += bit_1_count_lut[DMBR1[j] & DMBR2[j]];
        if(tmp_sum == 0) // tmp_sum is zero
            return 0.0;
        else
            overlap *= static_cast<double>(tmp_sum) / alphabet_sizes[i];
    }
    return overlap;
}

bool Node::set_equal(unsigned char* set1, unsigned char* set2, int byte_size)
{
    int i;
    for(i = 0; i < byte_size; i++)
        if(set1[i] != set2[i])return false;
    return true;
}

void Node::set_intersect(unsigned char* set1, unsigned char* set2, int byte_size, unsigned char* intersect_set, bool& is_intersect)
{
    int i;
    is_intersect = false;
    for(i = 0; i < byte_size; i++)
    {
        intersect_set[i] = set1[i] & set2[i];
        if(intersect_set[i] > 0)is_intersect = true;
    }
}

void Node::bitmap_to_letters(unsigned char* bitmap_set, int byte_size, unsigned char* letters, int &letter_count)
{
    int i, j;
    int tmp_letter_inc;
    int tmp_letter;
    letter_count = 0;
    tmp_letter_inc = 0;
    for(i = 0; i < byte_size; i++)
    {
        for(j = 0; j < BITS_PER_BYTE; j++)
            if(bitmap_set[i] & MASKS[j])
            {
                tmp_letter = tmp_letter_inc + j;
                letters[letter_count] = tmp_letter;
                letter_count++;            
            }
        tmp_letter_inc += BITS_PER_BYTE;
    }
}

int Node::set_size(unsigned char* bitmap_set, int byte_size)
{
    int i;
    int letter_count = 0;

    for(i = 0; i < byte_size; i++)
        letter_count += bit_1_count_lut[bitmap_set[i]];

    return letter_count;
}

void Node::log_DMBR(const unsigned char* const DMBR)
{
    int tmp_sum;
    unsigned char tmp_DMBR[DMBR_SIZE];

    for(int i=0;i<DMBR_SIZE;i++)
        tmp_DMBR[i] = DMBR[i];

    for(int i = 0; i < DIM; i++)
    {
        tmp_sum = 0;
        for(int j = DMBR_start_byte_lut[i]; j <= DMBR_end_byte_lut[i]; j++)
            tmp_sum += bit_1_count_lut[DMBR[j]];
        logO.log2File(tmp_sum);/*logO.log2File(" ");*/
    }
    logO.log2File("\n");

}

void Node::log_OneDirEntry_OnOneDscDim(  unsigned char*  DMBR, int dimNum,int* alphabet_sizes )
{


	unsigned char alphabet2[MAX_ALPHABET_SIZE]; // letters that appear in all components.  May be a subset of the current alphabet
	int size_of_alphabet2 = 0;


	assert(dimNum<DIM);

	Node::bitmap_to_letters(&(DMBR[dimNum*BYTES_PER_DIM_IN_DMBR]), BYTES_PER_DIM_IN_DMBR, alphabet2, size_of_alphabet2);


	for(int j=0;j<size_of_alphabet2;j++)
	{
		logO.log2File((int)alphabet2[j]);logO.log2File(" ");
	}

}



#endif
