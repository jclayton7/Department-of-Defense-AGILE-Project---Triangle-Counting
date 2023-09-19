// File: graphGen.h
// Author: Dr. Stephen Wheat, Austin Lehman, Samuel Udall, Michael Vandusen, Andrew Westlund
// Date: 9/16/22
// Pupose: Will generate a random graph based on the passed in vaules.
#include "graph.h"
#ifndef __GRAPHGEN_H
#define __GRAPHGEN_H

// Create typedefs for variables
typedef unsigned long long UINT64;
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;

typedef long long INT64;
typedef int INT32;
typedef short INT16;
typedef char INT8;

class graphGen {
	public:
		// Constructor
		graphGen(UINT64 vertexCount, UINT64 edgeFactor, UINT32 seed, UINT16 strict, bool weight, graph &Graph);
		// Destructor
		~graphGen();
		
		
	private:
		bool Weighted;
		UINT64 vCount;
		UINT64 *edgeList;
		graph *Graph;		// pointer for the graph object
		vID pID;
		vID eID;
		
		// Preconditions: strict bool is set to false; vertexCount and edgeFactor are greater than zero.
		// Postconditions: creates a random graph. 
		void randGraph(UINT64 vNum, UINT64 edgeFactor);
		
		
		
		// Preconditions: strict int is set to 1; vertexCount and edgeFactor are greater than zero.
		// Postconditions: creates a random graph. 
		void strictOutGraph(UINT64 vNum, UINT64 edgeFactor);
		
		// Preconditions: strict int is set to 2; vertexCount and edgeFactor are greater than zero.
		// Postconditions: creates a random graph. 
		void strictInGraph(UINT64 vNum, UINT64 edgeFactor);
};
#endif