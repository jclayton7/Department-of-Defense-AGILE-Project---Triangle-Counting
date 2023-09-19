// File: graphGen.cpp
// Author: Dr. Stephen Wheat, Samuel Udall, Michael Vandusen, Andrew Westlund
// Date: 9/16/22
// Pupose: Will generate a random graph based on the passed in vaules.

#include <iostream>			// Provides cout
#include <math.h>			// Provides math functions
#include "graphGen.h"		// Provides header file containing definitions
#include "graph.h"			// Provides graph object.

using namespace std;

graphGen::graphGen(UINT64 vertexCount, UINT64 edgeFactor, UINT32 seed, UINT16 strict, bool weight, graph &Graph) {
	vCount = round(pow(2, vertexCount));
	Weighted = weight;
	edgeList = new UINT64 [3];
	this->Graph = &Graph;				// pointer to the graph object
	
	if (seed == -1) {
		srand((unsigned) time(NULL)); 	// sets the rand seed to the time, so true random.
	} else {
		srand((unsigned) seed);			// sets passed in rand seed.
	}
	
	switch (strict) {		// chose which function to use
		case 0:
			randGraph(vertexCount,edgeFactor);
			break;
		case 1:
			strictOutGraph(vertexCount,edgeFactor);
			break;
		case 2:
			strictInGraph(vertexCount,edgeFactor);
			break;
		default: 
			cout << "Error with strict value\n";
			abort();
	}
	
}

graphGen::~graphGen() {
	
}

void graphGen::randGraph(UINT64 vNum, UINT64 edgefactor){
    // the number of vertices
	
	// Set number of edges
    long long edgeNum = edgefactor * vCount;
    // set Probabilities
    double A, B, C;
    A = 25;
    B = 15;
    C = 35;

    // Create index arrays
    
    int ab = A + B;
    int c_norm = round((C/(100 - (A + B))) * 100);
    int a_norm =round((A/(A + B)) * 100);

    bool ii_bit;
    bool jj_bit;
	bool max;
	edgeList[2] = 0;
	
    for (long long j = 0; j < edgeNum; j++){
        edgeList[0] = 0;		// starting vertex.
        edgeList[1] = 0;		// edge goes to this vertex.
        for (int i = 0; i < vNum; i++){
			max = false;	// for preventing self loops 
			// random math generation made by Michael Vandusen
            ii_bit = rand() % 100 > ab;
            jj_bit = rand() % 100 >(c_norm * ii_bit + a_norm * !ii_bit);
            edgeList[0] = edgeList[0] + pow(2, i) * ii_bit;
            edgeList[1] = edgeList[1] + pow(2, i) * jj_bit;
			
			pID.id = edgeList[0];		// makes the ids to add to graph.
			eID.id = edgeList[1];
			if (pID.id == eID.id) {		// prevents it from going to itself
				if (eID.id < vCount) {
					eID.id = eID.id + 1;
				} else {					// if eID.id is at the max number of vertices
					eID.id = eID.id - 1;
				}
			}
			if (Weighted) {
				edgeList[2] = rand() % 99 + 1;
			}
			Graph->addEdge(pID, eID, edgeList[2]);
        } 
	}
}

void graphGen::strictOutGraph(UINT64 vNum, UINT64 edgefactor){
    // Create index arrays
	bool maxed;
	edgeList[2] = 0;
	for (int i = 0; i < edgefactor; i++) {
		for (int j = 0; j < vCount; j++) {
			edgeList[0] = j;
			edgeList[1] = rand() % vCount;
			pID.id = edgeList[0];		// makes the ids to add to graph.
			eID.id = edgeList[1];
		
			if (Weighted) {						
				edgeList[2] = rand() % 99 + 1;
			}
			maxed = false;		// if the edge is going to itself or already exists
			while(pID.id == eID.id || !Graph->addEdge(pID, eID, edgeList[2])) {	
				if (eID.id < (vCount - 1) && !maxed) {
					eID.id = eID.id + 1;
				} else {					// if eID.id is at the max number of vertices
					eID.id = eID.id - 1;
					maxed = true;
				}
			}
		}
	}
}

void graphGen::strictInGraph(UINT64 vNum, UINT64 edgefactor){
    // Create index arrays
	bool maxed;
	edgeList[2] = 0;
	for (int i = 0; i < edgefactor; i++) {
		for (int j = 0; j < vCount; j++) {
			edgeList[1] = j;
			edgeList[0] = rand() % vCount;
			pID.id = edgeList[0];		// makes the ids to add to graph.
			eID.id = edgeList[1];
		
			if (Weighted) {						
				edgeList[2] = rand() % 99 + 1;
			}
			maxed = false;		// if the edge is going to itself or already exists
			while(pID.id == eID.id || !Graph->addEdge(pID, eID, edgeList[2])) {		
				if (pID.id < (vCount - 1) && !maxed) {
					pID.id = pID.id + 1;
				} else {					// if eID.id is at the max number of vertices
					pID.id = pID.id - 1;
					maxed = true;
				}
			}
		}
	}
}
