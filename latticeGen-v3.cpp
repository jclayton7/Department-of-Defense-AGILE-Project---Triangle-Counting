// File Name: latticeGen-v3.cpp
// Author: Judah Clayton and Stephen R. Wheat
// Date: 1/30/23
// Generates the lattice graph of triangles.

#include <cstdio>
#include <ctype.h>			// Provides isdigit
#include <iostream>			// Provides cout
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>            // Provides OpenMP functionality
#include "graph.h"

#define THREAD_COUNT 72

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
	cerr << "The program has started." << endl << endl;
	
	// Capture the dimensions of the lattice and calculate the size of the graph.
	UINT32 height = 1000;
	UINT32 width = 15000;
	UINT64 graphSize = height * width;
	UINT64 edgesCreated; // This is for the edges/second calculation.
	UINT32 edgesPerSecond;
	UINT64 correctTravEdges;
 
	// Lattice size.
	
	// Create the output arrays.
	double *output = new double[10]; // This array is for small numbers.
    UINT32 *output2 = new UINT32[6]; // This array is for large numbers.

    // Set the number of threads to be used and print it.
	omp_set_num_threads(THREAD_COUNT);

    // Create a file to output to.
	fstream file; // (https://www.geeksforgeeks.org/csv-file-management-using-c/)
	
    // Print the lattice size for clarity.
    output[0] = height;
	output[9] = width;


	/* From sugraphm11-21.cpp by Sam Udall, et al. */
	
	//Timer variables (Sam Udall, et al.)
	long seconds; // (Sam Udall, et al.)
    long microseconds; // (Sam Udall, et al.)
    double elapsed; // (Sam Udall, et al.)
	double elapsed_graphCreation;
	double elapsed_edgeCreation;
	double elapsed_triangleCounting;
    struct timeval begin, end; // (Sam Udall, et al.)
	
    /* (Sam Udall, et al.) */
	
	
	// Start the stopwatch.
	gettimeofday(&begin, 0); //(Sam Udall, et al.)
	
	// Initialize the graph.
	graph *lGraph = new graph(graphSize);

	// Stop the stopwatch.
	gettimeofday(&end, 0); //(Sam Udall, et al.)

    seconds = end.tv_sec - begin.tv_sec; //(Sam Udall, et al.)
    microseconds = end.tv_usec - begin.tv_usec; //(Sam Udall, et al.)
    elapsed_graphCreation = seconds + microseconds*1e-6; //(Sam Udall, et al.)
    output[4] = elapsed_graphCreation;

    // Zero out the timer variables just to be safe.
	end.tv_sec = 0;
	begin.tv_sec = 0;
	seconds = 0;
	microseconds = 0;
	
	// Start the stopwatch to time the edge creation.
    gettimeofday(&begin, 0); //(Sam Udall, et al.)
	
	// Create the horizontal edges.
	    // Create two indeces for the edges.
		vID vOne;
		vID vTwo;
		
        vOne.id	= 0;
        vTwo.id	= 0;
		
		// Increment over the height.
		for (UINT64 i = 0; i < height; i++) {
			vOne.id = width * i; // The starting vertex of each row.
			vTwo.id = vOne.id + 1;
			
			// Increment over the width.
			for (UINT64 j = 0; j < width - 1; j++) {
				vTwo.id = j + width * i + 1;				
				lGraph->addEdge(vOne, vTwo);
				edgesCreated++;
				vOne.id++;
			}
		}
				
	// Create both the upward and the downward vertical edges.
        vOne.id	= 0;
        vTwo.id	= 0;
		
	    for (UINT64 j = 0; j < width; j++) {
			vOne.id = j;
			
			for (UINT64 i = 0; i < height - 1; i++) {
				vTwo.id = j + width * (i + 1);
				lGraph->addEdge(vOne, vTwo);
				edgesCreated++;
				lGraph->addEdge(vTwo, vOne);
				edgesCreated++;
				vOne.id = vTwo.id; // Make the next-highest vertical vertex vOne.
                                   // so the next-highest edge can be created. 
			}
		}

	// Create the diagonal (bottom right to top left) edges.
		UINT64 rEndpt = width - 1; // This is the number of the last vertex in the first row.

		for (UINT64 j = rEndpt; j > 0; j--) {
			for (UINT64 i = 0; i < height - 1; i++) {
    			vOne.id = j + width * i; // Grab the bottom right vertex.
				vTwo.id = j + width * (i + 1) - 1; // Grab the top left vertex.
				lGraph->addEdge(vOne, vTwo);
				edgesCreated++;
			}
		} 
		
		
	// Stop the stopwatch for the edge creation.
   	gettimeofday(&end, 0); //(Sam Udall, et al.)

    // Calculate the edges/second.
	seconds = end.tv_sec - begin.tv_sec; //(Sam Udall, et al.)
    microseconds = end.tv_usec - begin.tv_usec; //(Sam Udall, et al.)
    elapsed_edgeCreation = seconds + microseconds*1e-6; //(Sam Udall, et al.)
    edgesPerSecond = edgesCreated / elapsed_edgeCreation;
    output2[0] = edgesPerSecond;

    // Zero out the timer variables just to be safe.
	end.tv_sec = 0;
	begin.tv_sec = 0;
	end.tv_usec = 0;
	begin.tv_usec = 0;
	seconds = 0;
	microseconds = 0;

    // Start the stopwatch to time the triangle counting.
    gettimeofday(&begin, 0); //(Sam Udall, et al.)
	
	// Count the triangles.
	lGraph->TC(output, output2);

	// Stop the stopwatch.
   	gettimeofday(&end, 0); //(Sam Udall, et al.)

    // Calculate the edges/second.
	seconds = end.tv_sec - begin.tv_sec; //(Sam Udall, et al.)
    microseconds = end.tv_usec - begin.tv_usec; //(Sam Udall, et al.)
    elapsed_triangleCounting = seconds + microseconds*1e-6; //(Sam Udall, et al.)
    output[3] = elapsed_triangleCounting;
	

    cerr << "Correct trust number: " << (height - 1) * (width - 1) * 2 << endl;
    cerr << "Correct cycle number: " << (height - 1) * (width - 1) * 2 << endl;

	// Print the recorded times.
	cerr << endl << endl << "Lattice size: " << output[0] << " X " << output[9] << endl;
    cerr << endl << "Number of threads: " << THREAD_COUNT << endl;
	cerr << endl << "Graph creation time: " << output[4] << " seconds." << endl;
    cerr << endl << "Edges per second: " << output2[0] << endl;
    
    cerr << endl << "cycleCount = " << output2[1] << endl;
    cerr << endl << "trustCount = " << output2[2] << endl;
    cerr << endl << "totalCount = " << output2[3] << endl;
    
    cerr << endl << "Time taken to count the cycle triangles: " << output[1] 
	     << " seconds." << endl;
    cerr << endl << "Time taken to count the trust triangles: " << output[2] 
	     << " seconds." << endl;
	cerr << endl << "Time taken for the triangle counting: " 
	     << output[3] << " seconds." << endl;

    cerr << endl << "Cycle GTEPS: " << output[6] << " GTEPS" << endl;
    cerr << endl << "Trust GTEPS: " << output[7] << " GTEPS" << endl;
    cerr << endl << "Total GTEPS: " << output[8] << " GTEPS" << endl;
    cerr << endl << "Total GTEPS (Voegele metric): " 
	     << (lGraph->getTotalDegree() / output[3]) * 1e-9 << " GTEPS" << endl;
           
    cerr << endl << "Cycle traversed edges: " << output2[4] << " edges" << endl;
    cerr << endl << "Trust traversed edges: " << output2[5] << " edges" << endl;
    cerr << endl << "Total traversed edges (Clayton): " << output2[4] + output2[5] << " edges" << endl << endl;
    cerr << endl << "Total traversed edges (Voegele): " << lGraph->getTotalDegree() << " edges" << endl << endl;
}