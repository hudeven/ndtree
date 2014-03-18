#include "ND_tree.h"
#include <vector>
#include "logClass.h"
#include <limits.h>
ofstream OutStream;

// GLOBAL VARIABLES - Added by Alok to make interface of this
// tree consistent with the rest of the trees.
string globalBQFilename = "randomBoxqueryAll.txt";
string globalRQFilename = "rangequeryAll.txt";
string globalDataFilename = "sourceData.txt";
string globalIndexFilename = "ndTree.dat";


//#define MAX_COPIES_OF_DATA_FILE 50

//double dir_min_util = 0.0000000000003;
const double dir_min_util = ((nodeSplitType==ORIGINAL)||(enforce_minUtil_for_exhaustSplit==1))?0.3:0.0000000000003;
const double leaf_min_util = 0.3;

int tmp_DMBR_byte_lut[DIM][MAX_ALPHABET_SIZE]; // Given a dim and a letter, store the coresponding byte in DMBR
int tmp_DMBR_bit_lut[DIM][MAX_ALPHABET_SIZE]; // Given a dim and a letter, store the coresponding bit in DMBR

int debug_boxQ_leaf_accessed=0;
int debug_boxQ_leaf_hit=0;
int debug_boxQ_leaf_hit_peak=0;
vector<int> debug_boxQ_leaf_hit_for_all;

logClass logO;

int debug_height;

int duplicateDataPoints; //used in batchBuild(...), batchGrow(...)

void LocalDMBRInfoCalculation()
{
    int tmp_byte = 0;
    int i, j;
    for(i = 0; i < DIM; i++)
    {
        //tmp_DMBR_start_byte_lut[i] = tmp_byte;

        for(j = 0; j < A[i]; j++)
        {
            tmp_DMBR_byte_lut[i][j] = tmp_byte + j / BITS_PER_BYTE;
            tmp_DMBR_bit_lut[i][j] = j % BITS_PER_BYTE; ////letters must starts from 0
        }


        /** how many bytes occupied by this DBMR[i][j] **/
        tmp_byte += (A[i] - 1) / BITS_PER_BYTE + 1;

        //tmp_DMBR_end_byte_lut[i] = tmp_byte - 1;
    }



}

Dir_entry makeRandomBoxQueryData(ifstream & query_file)
{
    Dir_entry dirEntry;
    for(int j = 0; j < DMBR_SIZE; j++)
        dirEntry.DMBR[j]=0;
    string line;

    for(int j=0;j<DIM;j++)
    {
        int byte_no,bit_no;
        getline(query_file, line);
        istringstream instr(line);
        int v;
        int box_size;
        instr>>box_size;
        for(int t=0;t<box_size;t++)
        {
            instr>>v;
            byte_no = tmp_DMBR_byte_lut[j][v];
            bit_no = tmp_DMBR_bit_lut[j][v];/** todo: here DMBR_bit_lut[j] should support enough letters **/
            dirEntry.DMBR[byte_no] |= MASKS[bit_no];
        }
    }
   
//Why to consume the rest lines. What are the float values? 
/* 	
    for(int i=0;i<MAX_DIM_AND_CONTDIM-DIM;i++)
        getline(query_file, line);
    //consumes the rest lines holding float values
    for(int i=0;i<MAX_DIM_AND_CONTDIM;i++)
        getline(query_file, line);
*/
    return dirEntry;
}

Dir_entry makeBoxQueryData(ifstream & query_file)
{
    Dir_entry dirEntry;

    for(int j = 0; j < DMBR_SIZE; j++)
        dirEntry.DMBR[j]=0;
    string line;

    for(int j=0;j<DIM;j++)
    {
        int byte_no,bit_no;
        getline(query_file, line);
        istringstream instr(line);
        int v;
        for(int t=0;t<boxSize;t++)
        {
            instr>>v;
            byte_no = tmp_DMBR_byte_lut[j][v];
            bit_no = tmp_DMBR_bit_lut[j][v];/** todo: here DMBR_bit_lut[j] should support enough letters **/
            dirEntry.DMBR[byte_no] |= MASKS[bit_no];
        }
    }
    for(int i=0;i<MAX_DIM_AND_CONTDIM-DIM;i++)
        getline(query_file, line);
    //consumes the rest lines holding float values
    for(int i=0;i<MAX_DIM_AND_CONTDIM;i++)
        getline(query_file, line);
    return dirEntry;
}

void batchGrow_with_duplicate(long skipSize, long sizeGrowTo)
{
    ND_tree ndt;
    Leaf_entry new_data/*, query_data, db_data*/;
    Error_code result;
    ndt.read_existing_tree(globalIndexFilename);
    ifstream data_file;
    data_file.open(globalDataFilename.c_str());
    if (data_file.fail())
    {
        cout<<"cant open file "<<globalDataFilename<<endl;
        exit(1);

    }
    int number_of_io = 0;
    long distinctDataPoints=0;
    string line;
    getline(data_file, line);
    for(long i = 0; i < sizeGrowTo; i++)
    {
#ifdef LOG_VECTOR_INDEX
logO.log2File("----------");logO.log2File(i);logO.log2File("\n");
#endif
#ifdef LOG_SPLITTING_VECTOR_INDEX
        vector_index = i;
#endif
        if(i<skipSize)
        {
            getline(data_file, line);
            continue;
        }
        else
        {
            stringstream instr(line);
           for(int j = 0; j < DIM; j++)
           {
               int n;
               instr >> n;
               new_data.key[j] = n;
           }
           new_data.record = 1; 
           result = ndt.insert_use_link(new_data, number_of_io);
           if(result == duplicate_error)
            {
                duplicateDataPoints++;
            }
            else
            {
                distinctDataPoints++;
            }
            getline(data_file, line);
        }
    }
    data_file.close();
    cout<<"Duplicate data points encountered:"<<duplicateDataPoints<<endl;
    cout<<"DistinctDataPoints data points indexed:"<<distinctDataPoints<<endl;
    cout<<"Total data points read:"<<sizeGrowTo <<endl;
    ndt.print_information( );
}

//this one only reads data file up to the give number of data points 
void batchBuild_with_duplicate(long  size)
{
    logO.clearLogs();
    duplicateDataPoints=0;
    ND_tree ndt;
    Leaf_entry new_data/*, query_data, db_data*/;
    Error_code result;
    ndt.create_empty_tree(A, dir_min_util, leaf_min_util, globalIndexFilename);
    ndt.read_existing_tree(globalIndexFilename);
    long num_of_points=size;
    ifstream data_file;
    data_file.open(globalDataFilename.c_str());
    if (data_file.fail())
    {
        cout<<"cant open file "<<globalDataFilename<<endl;
        exit(1);

    }

    int number_of_io = 0;
    string line;
    getline(data_file, line);
    int distinctDataPoints = 0;
    char n;
    for(long  i = 0; i < num_of_points && !data_file.eof(); i++)
    {
#ifdef LOG_VECTOR_INDEX
        logO.log2File("----------");logO.log2File(i);logO.log2File("\n");
#endif
#ifdef LOG_SPLITTING_VECTOR_INDEX
        vector_index = i;
#endif
        istringstream instr(line);
        for(int j = 0; j < DIM; j++)
        {
            instr >> n;
            new_data.key[j] = n;
        }
        new_data.record = 1; 
        result = ndt.insert_use_link(new_data, number_of_io);
        if(result == duplicate_error)
        {
            duplicateDataPoints++;
        }
        else
        {
            distinctDataPoints++;
        }
        getline(data_file, line);
    }
    data_file.close();
    cout<<"Duplicate data points encountered:"<<duplicateDataPoints<<endl;
    cout<<"DistinctDataPoints data points indexed:"<<distinctDataPoints<<endl;
    cout<<"Total data points read:"<<size <<endl;
    ndt.print_information( );
}

void batchRangeQuery(    )
{

    Error_code result;

    string input=globalIndexFilename;
    ND_tree ndt;
    Leaf_entry query_data;

    ndt.read_existing_tree(input);


                Leaf_entry query_results[QUERY_RESULTS_BUFFER_SIZE];
                int query_results_size;

    string query_fn = globalRQFilename;

    string str_num_of_points;
    int num_of_points;
    ////cout << endl << "How many query records:" << endl;
    ////getline(cin, str_num_of_points);
    ////num_of_points = string_to_int(str_num_of_points);

    num_of_points=TOTAL_RANGE_QUERY_NUM;

    for(int range=0;range<RANGE_STOP_BEFORE;range+=RANGE_SIZE_STEP)
    {
        //start = clock();

        ifstream query_file;
        query_file.open(query_fn.c_str(),ios::in);
        if (query_file.fail())
        {
            cout<<"cant open file "<<query_fn<<endl;
            exit(1);

        }                

        int number_of_io = 0;
        int total_number_of_io = 0;
        int total_results_size = 0;

        string line;
        int n;
        for(int i = 0; i < num_of_points; i++)
        {
            query_results_size = 0;

            //cout <<endl<<"range query for query point "<< i << endl;
            getline(query_file, line);

            if(query_file.bad()||query_file.fail() )

            {
                cout<<"reading query file error\n";
                exit(0);
            }

            istringstream instr(line);



            for(int j = 0; j < DIM; j++)
            {
                instr >> n;
                query_data.key[j] = n;
            }


            result = ndt.range_query_by_Hamming_dist(query_data, range, query_results, query_results_size, number_of_io);

            //cout << "  " << query_results_size << " " << number_of_io << endl;
            total_number_of_io += number_of_io;
            total_results_size += query_results_size;
        }
        query_file.close();

        //finish = clock();
        cout<<total_number_of_io<<" : "<<num_of_points<<endl;
        //elapsed_time = (double)(finish - start) / CLOCKS_PER_SEC;
        //cout << "Time (in seconds): " << elapsed_time / num_of_points << endl;
        cout << "For range: "<<range<<" range query I/O: " << static_cast<double>(total_number_of_io) / num_of_points << endl;
        cout << "Result size: " << static_cast<double>(total_results_size) / num_of_points << endl;




    }//end of for(int range=1;range<RANGE_STOP_BEFORE;++range)


    if(RUNNING_ENVIRONMENT == WINDOWS)
        system("pause");
}

void batchRandomBoxQuery()
{
    string input=globalIndexFilename;
    ND_tree ndt;
    ndt.read_existing_tree(input);
    LocalDMBRInfoCalculation();
// main box query loop in the original function
// begins here
    Leaf_entry query_results[QUERY_RESULTS_BUFFER_SIZE];
    int query_results_size;
    Dir_entry boxQueryData;

    string query_fn=globalBQFilename;
    int num_of_points=TOTAL_BOX_QUERY_NUM;
    ifstream query_file;
    query_file.open(query_fn.c_str());
    if (query_file.fail())
    {
        cout<<"cant open file "<<query_fn<<endl;
        exit(1);
    }

    debug_boxQ_leaf_hit_for_all.clear();
    debug_boxQ_leaf_accessed=0;

    int number_of_io ;
    int total_number_of_io = 0;

    int total_results_size=0;
    for(int i = 0; i < num_of_points; i++)
    {
        debug_boxQ_leaf_hit_peak=0;
        boxQueryData = makeRandomBoxQueryData(query_file);
        ndt.box_query(boxQueryData, query_results, query_results_size, number_of_io);
        total_number_of_io += number_of_io;
        total_results_size += query_results_size;
        debug_boxQ_leaf_hit_for_all.push_back(debug_boxQ_leaf_hit_peak);

    }


    query_file.close();
    cout<<"boxSize= RANDOM, AvG boxquery I/O: " << static_cast<double>(total_number_of_io) / num_of_points << endl;
    cout<<" AvG matched data point found="<< static_cast<double>(total_results_size)/num_of_points<< endl; 


    cout << " AvG leaf node accessed: " << static_cast<double>(debug_boxQ_leaf_accessed) / num_of_points << endl;

    assert(debug_boxQ_leaf_hit_for_all.size()==num_of_points);
    int debug_tatol=0;
    for(unsigned int i=0;i<debug_boxQ_leaf_hit_for_all.size();i++)
    {
        
        debug_tatol+=debug_boxQ_leaf_hit_for_all.at(i);
    
    }

    cout << " AvG leaf node hit peak: " << static_cast<double>(debug_tatol) / num_of_points << endl;
    
    if(RUNNING_ENVIRONMENT == WINDOWS)
        system("pause");
}

void batchBoxQuery()
{
    string input=globalIndexFilename;
    ND_tree ndt;
    ndt.read_existing_tree(input);
    LocalDMBRInfoCalculation();

    for(boxSize=BOX_SIZE_START_AT;boxSize<BOX_SIZE_STOP_BEFORE;boxSize+=BOX_SIZE_STEP)
    {
        //logO.clearLogs();
        Leaf_entry query_results[QUERY_RESULTS_BUFFER_SIZE];
        int query_results_size;


        Dir_entry boxQueryData;

        string query_fn="c:\\temp\\boxqueryAll.txt";
        if(RUNNING_ENVIRONMENT == UNIX)
            query_fn="boxqueryAll.txt";
        else
            query_fn="C:\\temp\\boxqueryAll.txt";
        int num_of_points=TOTAL_BOX_QUERY_NUM;

        ifstream query_file;
        query_file.open(query_fn.c_str());
    if (query_file.fail())
    {
        cout<<"cant open file "<<query_fn<<endl;
        exit(1);

    }

        debug_boxQ_leaf_hit_for_all.clear();
        debug_boxQ_leaf_accessed=0;

        int number_of_io ;
        int total_number_of_io = 0;

        int total_results_size=0;
        for(int i = 0; i < num_of_points; i++)
        {
            debug_boxQ_leaf_hit_peak=0;
            boxQueryData = makeBoxQueryData(query_file);
            ndt.box_query(boxQueryData, query_results, query_results_size, number_of_io);
            total_number_of_io += number_of_io;
            total_results_size += query_results_size;
            debug_boxQ_leaf_hit_for_all.push_back(debug_boxQ_leaf_hit_peak);

        }


        query_file.close();
        cout<<"boxSize= "<<boxSize<< " AvG boxquery I/O: " << static_cast<double>(total_number_of_io) / num_of_points << endl;
        cout<<" AvG matched data point found="<< static_cast<double>(total_results_size)/num_of_points<< endl; 


        cout << " AvG leaf node accessed: " << static_cast<double>(debug_boxQ_leaf_accessed) / num_of_points << endl;

        assert(debug_boxQ_leaf_hit_for_all.size()==num_of_points);
        int debug_tatol=0;
        for(unsigned int i=0;i<debug_boxQ_leaf_hit_for_all.size();i++)
        {
            
            debug_tatol+=debug_boxQ_leaf_hit_for_all.at(i);
        
        }

        cout << " AvG leaf node hit peak: " << static_cast<double>(debug_tatol) / num_of_points << endl;
    }
    if(RUNNING_ENVIRONMENT == WINDOWS)
        system("pause");
}


vector< vector<int> >  makeBoxQueryData_for_linearScan(ifstream & query_file)
{

    //int maxDimAndContDim = 16;//01/09/2007


    string line;

    vector< vector<int> >  boxQueryData;
    boxQueryData.resize(DIM);

    for(int j=0;j<DIM;j++)
    {

        getline(query_file, line);
        istringstream instr(line);
        boxQueryData[j].clear();

        int v;
        for(int t=0;t<(boxSize*A[j])/10;t++)
        {
            instr>>v;
            boxQueryData[j].push_back(v);

        }
    }


    for(int i=0;i<MAX_DIM_AND_CONTDIM-DIM;i++)
        getline(query_file, line);


    //consumes the rest lines holding float values
    for(int i=0;i<MAX_DIM_AND_CONTDIM;i++)
        getline(query_file, line);

    assert(boxQueryData.size()==DIM);
    assert(boxQueryData[0].size()==boxSize);

    return boxQueryData;
}








//result of this LinearScanBoxQuery should be 
void LinearScanBoxQuery(int dataNUM)
{
//    Leaf_entry query_results [QUERY_RESULTS_BUFFER_SIZE];


//    int query_results_size;


    int db_size=dataNUM;
    string query_fn="c:\\temp\\boxqueryAll.txt";


    ifstream query_file;
    query_file.open(query_fn.c_str());
    if (query_file.fail())
    {
        cout<<"cant open file "<<query_fn<<endl;
        exit(1);

    }




    for(boxSize=1;boxSize<=9;boxSize+=1)
    {


        query_file.seekg(0, ios::beg);


        int num_of_points=200;

        int totalNumOfMatches=0;

        for(int i = 0; i < num_of_points; i++) // for every query point
        {

            vector< vector<int> > matchedPoints;
            matchedPoints.clear();

            vector< vector<int> >  boxQueryData;
            boxQueryData = makeBoxQueryData_for_linearScan(query_file);



            vector<int> db_data;

            string db_fn=globalDataFilename;
            ifstream db_file;
            db_file.open(db_fn.c_str());

    if (db_file.fail())
    {
        cout<<"cant open file "<<db_fn<<endl;
        exit(1);

    }




            for(int j = 0; j < db_size; j++)
            {

                //cout<<"query data"<<i<<"db data"<<j<<endl;
                db_data.clear();
                string line;
                getline(db_file, line);
                istringstream dbstr(line);
                for(int k = 0; k < DIM; k++)
                {
                    int n;
                    dbstr >> n;
                    db_data.push_back(n);
                }


                bool matchOnAllDim = true;        
                for(unsigned int s=0;(s<boxQueryData.size())&&matchOnAllDim ;s++)
                {
                    bool matchOnOneDim=false;
                    for(unsigned int x=0;x<boxQueryData[s].size();x++)    
                        if(boxQueryData[s][x]==db_data[s])
                            matchOnOneDim=true;

                    if(!matchOnOneDim)
                        matchOnAllDim=false;

                }



                if(matchOnAllDim)
                {
                    bool findDuplicate=false;
                    for(unsigned int i=0;i<matchedPoints.size();i++)
                        if(matchedPoints.at(i)==db_data)
                            findDuplicate=true;

                    if(!findDuplicate)
                    {
                        matchedPoints.push_back(db_data);
                        //for(int t1=0;t1<boxQueryData.size();t1++)
                        //{
                        //    for(int t2=0;t2<boxQueryData.at(t1).size();t2++)
                        //        cout<<boxQueryData.at(t1).at(t2)<<" ";
                        //    cout<<endl;
 
                        //}    

                        //for(int t2=0;t2<db_data.size();t2++)
                        //    cout<<db_data.at(t2)<<": ";
                        //cout<<endl;


                    }
                }


            } 
            db_file.close();
            totalNumOfMatches+=(int)matchedPoints.size();

        }// end of         for(int i = 0; i < num_of_points; i++) // for every query point




        cout<<"linear scan boxSize="<<boxSize<<endl;
        cout <<  "matched data # from linear scan:" << static_cast<double>(totalNumOfMatches) / num_of_points << endl;

    }
    query_file.close();



}

#define OPT_TREEFILE "-idxfile"
#define OPT_DSCDIM "-dscdim"
#define OPT_LOADFILE "-load_file"
#define OPT_BQFILE "-bqfile"
#define OPT_RQFILE "-rqfile"
#define OPT_RANGE "-range"
#define OPT_SKIP "-skip"
#define OPT_COUNT "-count"
#define OPT_NEW "-newtree"
#define OPT_HELP "-help"
#define UNDEF_STR "__UNDEF__"
#define UNDEF_LONG -999999
struct options
{
    // Name of the data file.
    string datafile;
    // Name of the index file
    string idxfile;
    string bqfile;// box query file
    string rqfile;// range query file
    float range;
    long skip;
    long count;
    bool newtree;
    bool help;
};

void get_options(int argc, char **argv, struct options *opt)
{
    string opt_idxfile(OPT_TREEFILE);
    string opt_load_file(OPT_LOADFILE);
    string opt_bqfile(OPT_BQFILE);
    string opt_rqfile(OPT_RQFILE);
    string opt_range(OPT_RANGE);
    string opt_newtree(OPT_NEW);
    string opt_skip(OPT_SKIP);
    string opt_count(OPT_COUNT);
    string opt_help(OPT_HELP);
    for(long i=0;i<argc;i++)
    {
        if(!opt_idxfile.compare(argv[i]))
        {
            opt->idxfile = argv[i+1];
            i++;
        }
        if(!opt_bqfile.compare(argv[i]))
        {
            opt->bqfile = argv[i+1];
            i++;
        }
        if(!opt_rqfile.compare(argv[i]))
        {
            opt->rqfile = argv[i+1];
            i++;
        }
        if(!opt_load_file.compare(argv[i]))
        {
            opt->datafile = argv[i+1];
            i++;
        }
        if(!opt_range.compare(argv[i]))
        {
            stringstream s(argv[i+1]);
            s>>(opt->range);
        }
        if(!opt_count.compare(argv[i]))
        {
            stringstream s(argv[i+1]);
            s>>(opt->count);
        }
        if(!opt_skip.compare(argv[i]))
        {
            stringstream s(argv[i+1]);
            s>>(opt->skip);
        }
        if(!opt_newtree.compare(argv[i]))
        {
            opt->newtree = true;
        }
        if(!opt_help.compare(argv[i]))
        {
            opt->help = true;
        }
    }
}

void display_help()
{
    cout<<"This implementation of BoND Tree was modified by Alok"<<endl;
    cout<<"It only works with discrete dimensions and performs random sized box queries."<<endl;
    cout<<"Please feel free to contact Alok at watvealo@cse.msu.edu for any comments"<<endl;
    cout<<"or suggestions"<<endl;
    cout<<"Currently supported options are "<<endl;
    cout<<"1) "<<OPT_TREEFILE<<" : Specifies the template for index filenames. As there"<<endl;
    cout<<"\tare multiple files created, each file will have this name as the prefix"<<endl;
    cout<<"and be appended with the suitable suffix (0,1,2...)"<<endl;
    cout<<"2) "<<OPT_LOADFILE<<" : Specifies the data file to be loaded in the index"<<endl;
    cout<<"\tDatafile must be in CSV format."<<endl;
    cout<<"3) "<<OPT_BQFILE<<" : Name of the box query file"<<endl;
//  cout<<"\t\tBox query is specified as x1,x2,x3:y1,y2:z1"<<endl;
//  cout<<"\t\t\tHere, x1..x3 are characters range along first dimension, y1-y2 is range along second"<<endl;
//  cout<<"dimension and so on"<<endl
    cout<<"4) "<<OPT_RQFILE<<" : Name of the range query file"<<endl;
    cout<<"5) "<<OPT_RANGE<<" : Radius of the range query"<<endl;
    cout<<"6) "<<OPT_SKIP<<" : number of records in the data file that should be skipped"<<endl;
    cout<<"7) "<<OPT_COUNT<<" : Number of data records to be loaded in the database"<<endl;
    cout<<"8) "<<OPT_NEW<<" : Flag indicating that a new index file should be created"<<endl;
    cout<<"\tANY EXISTING INDEX FILE WILL BE DELETED"<<endl;
}


int main(int argc, char *argv[])
{
    if(nodeSplitType==ORIGINAL)
    {
        assert(dir_min_util>0.1);
        TREE_TYPE = STATIC_TREE;
    }
    options opt;
    //Initialize options to Undef values
    // Name of the data file.
    opt.datafile = "data/data_random";
    // Name of the index file
    opt.idxfile = "data/index_random";
    opt.bqfile = "data/box_query_random";
    opt.rqfile = UNDEF_STR;
    opt.range = 0.;
    opt.skip = 0;
    opt.count = LONG_MAX;
    opt.newtree = true;
    opt.help = false;
    get_options(argc, argv, &opt);

    if(opt.idxfile.compare(UNDEF_STR))
    {
    //    logO.log2File(opt.idxfile.insert(0, "Index will be created in the file ").c_str());
        cout<<"Index will be created in "<<opt.idxfile<<endl;
        globalIndexFilename = opt.idxfile;
    }
    if(opt.datafile.compare(UNDEF_STR))
    {
        globalDataFilename = opt.datafile;
      //  logO.log2File(opt.datafile.insert(0, "Source data file : ").c_str());
        cout<<"Source data file :"<<opt.datafile<<endl;
        cout<<"Number of records to load "<<opt.count<<endl;
        if(opt.newtree)
        {
        //    log0.log2File("Creatung a new file");
            cout<<"Creating a new index file"<<endl;
            batchBuild_with_duplicate(opt.count);
        }
        else
        {
        //    log0.log2File("Modifying existing file");
            cout<<"Modifying an existing file "<<endl;
            batchGrow_with_duplicate(opt.skip, opt.count);
        }
    }
    if(opt.bqfile.compare(UNDEF_STR))
    {
        globalBQFilename = opt.bqfile;
        //logO.log2File(opt.datafile.insert(0, "Box query file : ").c_str());
        cout<<"Box query file "<<opt.bqfile<<endl;
        batchRandomBoxQuery();
    }
    
    if(opt.rqfile.compare(UNDEF_STR))
    {
        globalRQFilename = opt.rqfile;
        //logO.log2File(opt.datafile.insert(0, "Range query file : ").c_str());
        cout<<"Range query file "<<opt.rqfile<<endl;
        batchRangeQuery();
    }
}
