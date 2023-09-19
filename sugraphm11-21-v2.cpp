// File: sugraphm11-21-v2.cpp
// Author: Michael VanDusen and Judah Clayton
// Date: 2/22/22
// Pupose: To test the graph program made by the ORU Research team

#include <cstdio>
#include <ctype.h>			// Provides isdigit
#include <iostream>			// Provides cout
#include <time.h>
#include <unistd.h>			// 
#include <math.h>
#include <sys/time.h>
#include "graphGen.h"		// Provides graphGen
#include "COgraph.h"
#include <omp.h>            // Provides OpenMP functionality

#define THREAD_COUNT 24

using namespace std;

/*

The standard to be followed for the elements of output[]:
	
0 Height
1 Cycle counting time
2 Trust counting time
3 Time taken for the triangle counting
4 Graph creation time 
5 Total gigs used
6 cycleGTEPS
7 trustGTEPS
8 totalGTEPS
9 Width


The standard to be followed for the elements of output2[]:
	
0 Edges per second
1 cycleCount
2 trustCount
3 totalCount
4 Cycle traversed edges
5 Trust traversed edges

*/

int main(int argc, char **argv) {
    
    clock_t clock0 = clock();//Start of the program
    
    vID z;
    
    clock_t clock1 = clock();//Graph has been assigned
    
// --------- Judah's output arrays ---------------------    

	double *output = new double[10]; // This array is for small numbers.
    UINT32 *output2 = new UINT32[6]; // This array is for large numbers.

// --------- Graph and generater variable defaults ---------------------
   
	bool backEdge = false;
	bool dumpToFile = false;
	bool generate = false;
	bool isFile = false;
	bool weight = false;
	
	//Timer variaables
    long seconds;
    long microseconds;
    double elapsed;
    double elapsed_triangleCounting;
    struct timeval begin, end;
	
	INT16 opt;
	UINT16 allocation = 1;
	UINT16 strict = 0;
	UINT16 print10 = 10;
	
	UINT32 seed = -1;
	
	UINT64 genEdgeCount = 16; // Edited by Judah Clayton
	UINT64 genVertexCount = 16777216; // Edited by Judah Clayton. This is 2^24.
	UINT64 vNum = 32000000;			

	char *file = NULL;
	
	opterr = 0;

// --------------------  getop flags----------------------------------

	while ((opt = getopt(argc, argv, "bdgihswa:c:e:f:v:")) != -1) {
		switch (opt) {
			case 'b':
				backEdge = true;
				break;
			case 'd':
				dumpToFile = true;	//dump the sorted graph to output file. 
				break;
			case 'g':	
				if (!isFile) { 
					generate = true;
					vNum = round(pow(2,genVertexCount));	// incase the user never calls -v flag
					break;
				} else {
					fprintf(stderr, "The -g flag can not be used with the -f flag.\n");
					abort();
				}
			case 'c':
				if (generate) {				// graph generator random() seed
					if (isdigit(*optarg)) {
						seed = atoi (optarg);
						break;
					} else {
						fprintf(stderr, "Option -c requires an integer value.\n");
						abort();
					}
				} else {
					fprintf(stderr, "Option -c requires the -g flag to be called first.\n");
					abort();
				}
			case 'e': 
				if (generate) {
					if (isdigit(*optarg)) {
						genEdgeCount = atoi (optarg);
						break;
					} else {
						fprintf(stderr, "Option -e requires an integer value.\n");
						abort();
					}
				} else {
					fprintf(stderr, "Option -e requires the -g flag to be called first.\n");
					abort();
				}
			case 's': 
				if (generate) {
					strict = 1;
					break;
				} else {
					fprintf(stderr, "Option -s requires the -g flag to be called first.\n");
					abort();
				}
			case 'i':
				if (strict == 1) {
					strict = 2;
					break;
				} else {
					fprintf(stderr, "Option -i requires the -s flag to be called first.\n");
					abort();
				}
			
			case 'v':
				if (generate) {
					if (isdigit(*optarg)) {
						genVertexCount = atoi (optarg);
						vNum = round(pow(2,genVertexCount));
						break;
					} else {
						fprintf(stderr, "Option -v requires an integer value.\n");
						abort();
					}
				} else {
					fprintf(stderr, "Option -v requires the -g flag to be called first.\n");
					abort();
				}
			case 'w':
				weight = true;
				break;
			case 'h':
				cout << "Command Flag Options:" << endl;
				cout << "-b:  your wanting to have back edge functionality. not fully implemented (always true)." << endl;
				cout << "-d:  state that you want to dump the graph to an output file. " << endl;
				cout << "-g:  state that you want the graph to be generated." << endl;
				cout << "\t-v:  [UINT16 genVertexCount]: number vertices are generated, default 4; requires -g to be called first." << endl;
				cout << "\t-e:  [UINT16 edgefactor]:  each vertex will have an average of edgefactor edges, default 4; requires -g to be called first." << endl;
				cout << "\t-s:  each vertex will have exactly edgefactor edges in the outdegree; requires -g to be called first." << endl;
				cout << "\t\t-i: each vertex will have exactly edgefactor edges in the indegree; requires -s to be called first." << endl;
				cout << "\t-c:  set the seed for generation, default is time function (truely random); requires -g to be called first." << endl;
				cout << "-a:  set the array allocation rate, must enter an integers, defualt is 1." << endl;
				cout << "-w:  gives weights to each edge." << endl; 
				cout << "-f:  use a file name and the path to get there." << endl;
				abort();
			case 'f':
				if (!generate) {
					isFile = true;
					file = optarg;
					break;
				} else {
					fprintf(stderr, "Option -f can not be used with the -g flag.\n");
					abort();
				}
			case 'a':
				if (isdigit(*optarg)) {
					allocation = atoi (optarg);
					break;
				} else {
					fprintf(stderr, "Option -a requires an integer value.\n");
				}
			case '?':
				if (optopt == 'f') {
					fprintf(stderr, "Option -%c requires the path and file name you are tring to use\n", optopt);
				} else if (optopt == 'e' || optopt == 'v' || optopt == 'a') {
					fprintf(stderr, "Option -%c requires an integer value.\n", optopt);
				}
			default:
				abort();
		}
	}
	for (int i = optind; i < argc; i++) {
		printf ("Non-option argument %s\n", argv[i]); 
	}
   
   if(!generate && !isFile) {		// if no input flag given, do default graph generater
		generate = true;
		vNum = round(pow(2,genVertexCount));
	}
	
	if (pow(2, genVertexCount) <= genEdgeCount) {		// if user asks for more edges than there is vertices,
		genEdgeCount = genVertexCount;					// have edges set to the square root of number of vertices.
	}
	
   	if (vNum < 10) {		// if you have less than 10 vertices print the amount you have.
		print10 = vNum;
	}
// --------------------- Init Graph ----------------------------------------------------
   
    graph *myGraph = new graph(vNum);
	myGraph->setAllocation(allocation);
// ----------------------- populate Graph ------------------------------------------------	
	
	if (generate) {
		cout << "Start of Generation" << endl;
		graphGen myGen(genVertexCount, genEdgeCount, seed, strict, weight , *myGraph);
		cout << "End of Generation\n" << endl;
	} else {
		cout << "Start of Read" << endl;
		vID pID;
		vID eID;
		unsigned int *A = new unsigned int[3];
		
		FILE * pfile;
		pfile = fopen(file,"rb");
		fseek(pfile,0,SEEK_END);
		unsigned long long len = ftell(pfile);
		cout << "The length of the file is " << len << " Bytes." <<endl;
        //cerr << "Before rewind" << endl;
		rewind(pfile);
		//cerr << "After rewind" << endl;
    
        while(!feof(pfile)) { // Edit made by Judah Clayton (original was "for(int n =0; n < len/8; n++){"
        
            //cerr << "A[0]: " << A[0] << " A[1]: " << A[1] << " A[2]: " << A[2] << endl;
            fread(A,4,3,pfile); // Edit made by Judah Clayton. Original was "fread(A,4,2,pfile);"
            //cerr << "pID.id = A[0];" << endl;
    		pID.id = A[0];
    		//cerr << "eID.id = A[1];" << endl;
    		eID.id = A[1];
    		//cerr << "myGraph->addEdge(pID,eID);" << endl;
            //cerr << "pID.id = " << pID.id << endl;
            //cerr << "eID.id = " << eID.id << endl;
            //cerr << "WEIGHT = " << A[2] << endl << endl; // Addition made by Judah Clayton.

            // The graph500 graph file graph500-scale24-ef16_adj.tsv has a format that is not that which program was designed for.
            // It has a line for every element of the adjacency matrix of the graph, with nonzero or zero weights signifying
            // whether an edge exists between the two vertices on the line. Therefore, an edge should only be added when the 
            // weight is nonzero. Addition made by Judah Clayton.
            //cerr << "A[2] = " << A[2] << endl;
    		if(A[2] > 0) { // Addition made by Judah Clayton.
                myGraph->addEdge(pID,eID);
            }
    	}  
		cout << "End of Read" << endl;
	}
    clock_t clock2 = clock();//Graph has been populated
	
// ------------------------------ Use The Graph --------------------------------------------	
	
	if (dumpToFile) {
		myGraph->dumpGraph();
	}
	// for your use
	
	
	
    for(z.id = 0; z.id < print10;z.id++){
        myGraph->printVneighbors(z);
    }
	
// -------------------------------- Make Hisotgram --------------------------------------------
	
    clock_t clock3 = clock();// before the histogram calculations
    int max = 0;
    int maxID = 0;
    int maxV = myGraph->getVertexCount();
    unsigned int *outDegreeHist = new unsigned int[11];
    for (int i = 0; i < 11 ;i++){
        outDegreeHist[i] = 0;
    }
    // 0   1-20
    // 1   21-50
    // 2   51-100
    // 3   101-200
    // 4   201-400
    // 5   401-800
    // 6   800-1000
    // 7   1001-1500
    // 8   1501-2000
    // 9   2001-3185
    for(z.id = 0; z.id < maxV; z.id++) {
        if (myGraph->getVoutDegree(z) == 0) {
			outDegreeHist[10]++;
		}
		else if(myGraph->getVoutDegree(z) < 21 && myGraph->getVoutDegree(z) > 0){
            outDegreeHist[0]++;
        }
        else if(myGraph->getVoutDegree(z) < 51){
            outDegreeHist[1]++;
        }
        else if(myGraph->getVoutDegree(z) < 101){
            outDegreeHist[2]++;
        }
        else if(myGraph->getVoutDegree(z) < 201){
            outDegreeHist[3]++;
        }
        else if(myGraph->getVoutDegree(z) < 401){
            outDegreeHist[4]++;
        }
        else if(myGraph->getVoutDegree(z) < 801){
            outDegreeHist[5]++;
        }
        else if(myGraph->getVoutDegree(z) < 1001){
            outDegreeHist[6]++;
        }
        else if(myGraph->getVoutDegree(z) < 1501){
            outDegreeHist[7]++;
        }
        else if(myGraph->getVoutDegree(z) < 2001){
            outDegreeHist[8]++;
        }
        else{
            outDegreeHist[9]++;
        }
    }
    clock_t clock4 = clock(); // after Histogram
    for (z.id = 0; z.id < maxV; z.id++){		// find the vertex with the most eges outdegree
        if(max < myGraph->getVoutDegree(z)){
            max = myGraph->getVoutDegree(z);
            maxID = z.id;
        }    
    }
   
    clock_t clock5 = clock(); //after max
    
// --------------------- Print Histogram -----------------------------------------------

    cout << "Out Degree Histogram" << endl << endl;
	cout << "0             " << outDegreeHist[10] << endl;
    cout << "1 - 20        " << outDegreeHist[0] << endl;
    cout << "21 - 50       " << outDegreeHist[1] << endl;
    cout << "51 - 100      " << outDegreeHist[2] << endl;
    cout << "101 - 200     " << outDegreeHist[3] << endl;
    cout << "201 - 400     " << outDegreeHist[4] << endl;
    cout << "401 - 800     " << outDegreeHist[5] << endl;
    cout << "801 - 1000    " << outDegreeHist[6] << endl;
    cout << "1001 - 1500   " << outDegreeHist[7] << endl;
    cout << "1501 - 2000   " << outDegreeHist[8] << endl;
    cout << "2001+         " << outDegreeHist[9] << endl;

// ----------------------------- print clocks ----------------------------------------------
	cout << "\nTotal number of edge is " << myGraph->getTotalDegree() << endl;
    cout << "vertex " << maxID << " Has the Largest out degrees which is: " << max << endl;
	
	// print out the vertex with most edges (optional fill free to comment out)
	z.id = maxID;
    myGraph->printVneighbors(z);	
	
 
 
 
 
 
 
 
 
    srand(0);
    
    float ttime = (float)(clock1 - clock0) / (float)CLOCKS_PER_SEC;
    cout << "\n\nThe time taken to allocate the graph: " << ttime <<endl;
    ttime = (float)(clock2 - clock1) / (float)CLOCKS_PER_SEC;
    cout << "The time taken to populate the graph: " << ttime <<endl;
    ttime = (float)(clock4 - clock3) / (float)CLOCKS_PER_SEC;
    cout << "The time taken to make Histogram: " << ttime <<endl;
    ttime = (float)(clock5 - clock4) / (float)CLOCKS_PER_SEC;
    cout << "The time taken to find max: " << ttime <<endl;

	cout << "\nThe unused space is " << myGraph->getUnusedSpace() << endl;
	cout << "\n\n";
	


	
    // Set the number of threads to be used and print it.
	omp_set_num_threads(THREAD_COUNT);	

    // Start the stopwatch to time the triangle counting.
    gettimeofday(&begin, 0); //(Sam Udall, et al.)
	
	// Count the triangles.
	myGraph->TC(output, output2);

	// Stop the stopwatch.
   	gettimeofday(&end, 0); //(Sam Udall, et al.)

    // Calculate the edges/second.
	seconds = end.tv_sec - begin.tv_sec; //(Sam Udall, et al.)
    microseconds = end.tv_usec - begin.tv_usec; //(Sam Udall, et al.)
    elapsed_triangleCounting = seconds + microseconds*1e-6; //(Sam Udall, et al.)
    output[3] = elapsed_triangleCounting; 
    
    

	// Print the recorded times.
    cerr << endl << "Number of threads: " << THREAD_COUNT << endl;
    cerr << endl << "cycleCount = " << output2[1] << endl;
    cerr << endl << "trustCount = " << output2[2] << endl;
    cerr << endl << "totalCount = " << output2[3] << endl;
    cerr << endl << "Time taken for the triangle counting: " 
	     << output[3] << " seconds." << endl;
    cerr << endl << "Cycle GTEPS: " << output[6] << " GTEPS" << endl;
    cerr << endl << "Trust GTEPS: " << output[7] << " GTEPS" << endl << endl;
    cerr << endl << "Total GTEPS (Voegele metric): " 
	     << (myGraph->getTotalDegree() / output[3]) * 1e-9 << " GTEPS" << endl;
           // This GTEPS value is this program's performance 
           // according to the metric Voegele, et al. use 
           // (see "Triangle Counting: The Basis for a Future
           // Attempt at the AGILE Project").
    cerr << endl << "Cycle travEdges: " << output2[4] << " edges" << endl;
    cerr << endl << "Trust travEdges: " << output2[5] << " edges" << endl << endl;
    
    return(0);
}