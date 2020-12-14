#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "include/minirel.h"
#include "include/bufmgr.h"
#include "include/heapfile.h"
#include "include/join.h"
#include "include/relation.h"
#include <unordered_map>

int MINIBASE_RESTART_FLAG = 0;// used in minibase part

#define NUM_OF_DB_PAGES  2000 // define # of DB pages
#define NUM_OF_BUF_PAGES 50 // define Buf manager size.You will need to change this for the analysis

int main()
{
	Status s;


	//
	// Initialize Minibase's Global Variables
	//

	minibase_globals = new SystemDefs(s, 
		"MINIBASE.DB",
		"MINIBASE.LOG",
		NUM_OF_DB_PAGES,   // Number of pages allocated for database
		500,
		NUM_OF_BUF_PAGES,  // Number of frames in buffer pool
		NULL);
	
	//
	// Initialize random seed
	//

	srand(1);

	//
	// Create Random Relations R(outer relation) and S for joining. The definition is in relation.h, 
	// # of tuples: NUM_OF_REC_IN_R, NUM_OF_REC_IN_S in join.h
	//  
	//
    

	cerr << "Creating random records for relation R\n";
	CreateR();
	cerr << "Creating random records for relation S\n";
	CreateS();

	//
	// Initialize the specification for joins
	//

	JoinSpec specOfS, specOfR;

	CreateSpecForR(specOfR);
	CreateSpecForS(specOfS);
    
    HeapFile *S = new HeapFile ("S", s); // new HeapFile storing records of R
    
    string fileS = "/Users/alex/Desktop/S.txt";
    char charFile[1024];
    strcpy(charFile, fileS.c_str());
    
    //PrintS(S, charFile);

    HeapFile *R = new HeapFile ("R", s); // new HeapFile storing records of R
    
    string fileR = "/Users/alex/Desktop/R.txt";
    char charFile2[1024];
    strcpy(charFile2, fileR.c_str());
    
    //PrintR(R, charFile2);
    
    long pinRequests, pinMisses;
    double duration;
    
    
    
    TupleNestedLoopJoin(specOfS, specOfR, pinRequests, pinMisses, duration);
    
    HeapFile *SR = new HeapFile ("SXR", s); // new HeapFile storing records of R
    
    string fileSR = "/Users/alex/Desktop/SR.txt";
    char charFile3[1024];
    strcpy(charFile3, fileSR.c_str());
    
    PrintResult(SR, charFile3);
    

    int input;
    //Opens for reading the file
    ifstream SR_file ( "/Users/alex/Desktop/SR.txt" );
    //Reads one string from the file
//    SR_file >> first; SR_file >> first; SR_file >> first;  SR_file >> first; SR_file >> first;
//    //Should output 'this'
//    cout<< first <<"\n";
    
    unordered_map<int, int> nestedLoops;
    
    SR_file >> input;

    while (!SR_file.eof( ))      //if not at end of file, continue reading numbers
    {
        if(!nestedLoops.count(input))
            nestedLoops[input] = 1;
        else nestedLoops[input]++;
        
        SR_file >> input; SR_file >> input; SR_file >> input; SR_file >> input; SR_file >> input; SR_file >> input; SR_file >> input;
        
    }
    
    SR_file.close();
    
    cout << nestedLoops[12222] << endl;
    
    
	// 
	// Do your joining here.
	//
	
	//
	// The end.
	//
    
    //delete the created database
    remove("MINIBASE.DB");

	return 0;
}
