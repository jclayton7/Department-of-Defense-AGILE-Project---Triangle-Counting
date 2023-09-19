// File Name: tc.cpp
// Author: Judah Clayton and Stephen R. Wheat
// Date: 9/2/22
  
#include "graph.h"
#include <stdio.h>
#include <iostream>			// Provides cout
#include <sys/time.h>


using namespace std;

//******************************************************************************

UINT16 vertex::getMyPos() {
	return myPos;
}

//******************************************************************************

UINT16 vertex::getMyBackPos() {
	return myBackPos;
}

//******************************************************************************

int graph::TC(double *array) { // vertexList is the list of vID's of all 
                                // vertices in the graph.
    int rc = 0;
    UINT32 cycleCount = 0;
    UINT32 trustCount = 0;
    UINT32 totalCount = 0;
    
	
	//Timer variables (Sam Udall, et al.)
	long seconds; // (Sam Udall, et al.)
    long microseconds; // (Sam Udall, et al.)
    double elapsed; // (Sam Udall, et al.)
	double elapsed_cycle;
	double elapsed_trust;
	double cycleGTEPS;
	double trustGTEPS;
	UINT64 travEdges = 0; // This keeps track of the number of traversed edges.
    struct timeval begin, end; // (Sam Udall, et al.)





    // Count the cycle triangles and time it.

    // Start the stopwatch to time the edge creation.
    gettimeofday(&begin, 0); //(Sam Udall, et al.)
	
    cycleCount = cycle(travEdges);
    array[1] = cycleCount;

	// Stop the stopwatch.
   	gettimeofday(&end, 0); //(Sam Udall, et al.)

    // Calculate the GTEPS.
	seconds = end.tv_sec - begin.tv_sec; //(Sam Udall, et al.)
    microseconds = end.tv_usec - begin.tv_usec; //(Sam Udall, et al.)
    elapsed_cycle = seconds + microseconds*1e-6; //(Sam Udall, et al.)
    cycleGTEPS = (travEdges / elapsed_cycle)*1e-9; // The multiplication by 1e-9 puts the number in 
	                                               // GTEPS instead of TEPS.
    array[11] = cycleGTEPS;

    // Zero out travEdges for correct calculation.
	travEdges = 0;

    // Zero out the timer variables for correct calculation.
	end.tv_sec = 0;
	begin.tv_sec = 0;
	end.tv_usec = 0;
	begin.tv_usec = 0;
	seconds = 0;
	microseconds = 0;




    // Count the trust triangles and time it.

    // Start the stopwatch to time the edge creation.
    gettimeofday(&begin, 0); //(Sam Udall, et al.)
	
    trustCount = trust(travEdges);
    array[2] = trustCount;
	
	// Stop the stopwatch.
   	gettimeofday(&end, 0); //(Sam Udall, et al.)

    // Calculate the edges/second.
	seconds = end.tv_sec - begin.tv_sec; //(Sam Udall, et al.)
    microseconds = end.tv_usec - begin.tv_usec; //(Sam Udall, et al.)
    elapsed_trust = seconds + microseconds*1e-6; //(Sam Udall, et al.)
    trustGTEPS = (travEdges / elapsed_trust)*1e-9; // The multiplication by 1e-9 puts the number in 
	                                               // GTEPS instead of TEPS.
	array[12] = trustGTEPS;

    // Zero out the timer variables for correct calculation.
	end.tv_sec = 0;
	begin.tv_sec = 0;
	end.tv_usec = 0;
	begin.tv_usec = 0;
	seconds = 0;
	microseconds = 0;

    totalCount = cycleCount + trustCount;
    rc = totalCount;
	array[3] = totalCount;

    return(rc);
}

/*********************************************************************************************************/

UINT32 graph::cycle(UINT64 &tec) { // "tec" is the traversed edge count.
    UINT32 rc = 0; // rc will contain the number of triangles.
	UINT32 rc2 = 0; // rc2 will as well.
	
	// Create objects and pointers to work with the vertices and their neighbor lists.
	vertex *vrtxV;
	vID uVid;
	
	Edge *v_bList = NULL;
	Edge *v_nList = NULL;
	Edge *u_nList = NULL;
	UINT16 vInDeg; // This is how big v's bList is.
    UINT16 vOutDeg; // This is how big v's nList is.
	UINT16 uOutDeg; // This is how big u's nList is.
    UINT16 vMyPos; 
    UINT16 vMyBackPos;
	
	// Execute a loop over all vertices in the graph.
	for (UINT64 i = 0; i < vertexCount; i++) {
		// Access vertex v.
		vrtxV = &vList[i];
		
		// Access the backlist and neighbor list of v.
		v_bList = getBackNList(i);
		v_nList = getNList(i);
		
		vInDeg = vrtxV->getInDegree();
	    vOutDeg = vrtxV->getOutDegree();
	    vMyPos = vrtxV->getMyPos();
	    vMyBackPos = vrtxV->getMyBackPos();
				
	    // Execute a loop over each successor u of v.
		for (UINT32 j = vMyPos; j < vOutDeg; j++) {
            // Access vertex u.
            uVid = v_nList[j].vid;
			
	    	// Access the neighbor list of u.
    		u_nList = getNList(uVid.id);

            // Store u's outDegree.
			uOutDeg = vList[uVid.id].getOutDegree();
			
			// Add the cardinality of the intersection of v's back-successors  
			// and u's neighbors to the triangle count. Pass the v's position 
			// in its backlist and its in-degree as the first list's bounds
			// (only v's back-successors should be included in the intersection). Pass 
			// zero and u's out-degree as the second list's bounds (all of u's
			// neighbors should be included in the intersection).
			rc2 = intersectCard(v_bList, u_nList, vMyBackPos, 0, vInDeg, uOutDeg);
			rc += rc2;
			
			// Increment tec appropriately, using the lower and upper bounds from above
			// (the subtraction of lower bounds with a value of zero has been emitted).
			tec += vInDeg - vMyBackPos + 1; // Increment by the number of v's back-successors.
			tec += uOutDeg + 1; // // Increment by the number of u's neighbors.
		}
    }
	
	return(rc);
} // The algorithm implemented in this method came from from Stephen R. Wheat.


UINT32 graph::trust(UINT64 &tec) { // "tec" is the traversed edge count.
	UINT32 rc = 0; // rc will contain the number of triangles.
    UINT32 rc2 = 0;

	// Create objects and pointers to work with the vertices and their neighbor lists.
	// We don't need an pointer for vertex v because v's ID and i would always be equal.
	vID uVid;
	Edge *v_nList = NULL;
	Edge *u_nList = NULL;
	UINT16 vOutDeg = 0; // This is how big v's nList is.
	UINT16 uOutDeg = 0; // This is how big u's nList is.
	
	// Execute a loop for each vertex v.
	for (UINT64 i = 0; i < vertexCount; i++) {
        // Access vertex v and its neighbor list.
		v_nList = getNList(i);
		vOutDeg = vList[i].getOutDegree();

	    // Execute a loop for each neighbor u of v.
		for (UINT16 j = 0; j < vOutDeg; j++) {
            // Access vertex u.
            uVid = v_nList[j].vid;

	    	// Access the neighbor list of u.
    		u_nList = getNList(uVid.id);

            // Store u's outDegree.
			uOutDeg = vList[uVid.id].getOutDegree();

			// Add the cardinality of the intersection of v's neighbors  
			// and u's neighbors to the triangle count. Pass zero 
			// as the lower bounds and the outDegree's as the upper bounds (The list 
			// bounds need to be passed so only legitimate neighbors are included in the sets 
			// being intersected).
			rc2 = intersectCard(v_nList, u_nList, 0, 0, vOutDeg, uOutDeg);
			rc += rc2;
			
			// Increment tec appropriately, using the lower and upper bounds from above
			// (the subtraction of lower bounds with a value of zero has been emitted).
			tec += vOutDeg + 1; // Increment by the number of v's back-successors.
			tec += uOutDeg + 1; // Increment by the number of u's neighbors.
        }
	}
	
	return(rc);
} // The algorithm implemented in this method came from from Stephen R. Wheat.


UINT32 graph::intersectCard(Edge *nList1, Edge *nList2, UINT16 list1LB, UINT16 list2LB, 
                            UINT16 list1UB, UINT16 list2UB) {
	// Declare a return variable to hold the cardinality 
	// of the intersection.
	UINT32 rc = 0;
	
    // Declare the index counters and bounds.
	UINT32 ind1, ind2 = 0;
		
	// Check the indeces to make sure they're in the 
	// bounds of the lists.
    for (ind1 = list1LB, ind2 = list2LB; ind1 < list1UB && ind2 < list2UB;) {
		// Explanation of the for loop condition above: If one of the indeces has reached 
		// the end of its list (i.e. this index is equal to listXUB), it is not possible 
		// for the cardinality of the intersection to increas any more because the last 
		// element in the list is in index listXUB - 1.
		
		// Compare the first element of each list with each other.
		if (nList1[ind1].vid.id == nList2[ind2].vid.id) {	
			// If they're equal, increment both index counters
			// and increment the cardinality variable.
			ind1++;
			ind2++;
			rc++;
		} else if (nList1[ind1].vid.id < nList2[ind2].vid.id) {
			// If the first list's index has a 
			// lower value, increment it.
			ind1++;
		} else {
			// If the second list's index has a 
			// lower value, increment it.
			ind2++;
		}
    }
	
	return(rc);
}