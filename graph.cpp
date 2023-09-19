// File: graph.cpp
// Author: Dr. Stephen Wheat, Austin Lehman, Samuel Udall, Michael Vandusen, Andrew Westlund
// Date: 1/31/22
// Pupose: This program will recieve a graph and will create a list of all the vertices
// and will list all the vertices adjacent to each vertex.
// The both the list and the lists of adjacent vertices will be dynamically allocated
// and will be sorted least to greatest.

#include <cstdio>
#include <iostream>			// Provides cout
#include "graph.h"			// Provides header file containing definitions

using namespace std;

// Constructor for vertex object
vertex::vertex() {
    initVertex();
	initBackVertex();
}

vertex::~vertex() {
    delete [] nList;
	delete [] backNList;
}

void vertex::initVertex() {
    outDegree = 0;				// The vertex starts off with no outgoing edges
    capacity = 0;				// The list of adjacent vertices starts off empty
    myPos = 0;					
	
    // The list of adjacent vertices starts off empty
    nList = NULL;
}

void vertex::initBackVertex() {
	inDegree = 0;				// The vertex starts off with no incoming edges
	backCapacity = 0;			// The list of adjacent vertices starts off empty
	myBackPos = 0;
	backNList = NULL;
}

// This differs from initVertex, as we must deal with getting rid of a possible neighbor list
void vertex::clearVertex() {
    if (nList) {
		delete [] nList;
    }
	
    initVertex();
}

void vertex::clearBackVertex() {
	if (backNList) {
		delete [] backNList;
	}
	
	initBackVertex();
}

INT16 vertex::isNeighbor(vID v) {
    bool check = false;	// Equals false if n is not a neighbor
    INT16 mid;		
    INT16 lowerBound = 0;
    // Set the upper bound to start as the last possible position of the vertex list
    INT16 upperBound = outDegree - 1;
    
    // While the indicated id has not been found and the entire list has not been searched
    while (lowerBound <= upperBound) {
		mid = lowerBound + ((upperBound - lowerBound) / 2);
	
		if ((nList[mid]).vid.id > v.id) {
			upperBound = mid - 1;
		} else if ((nList[mid]).vid.id < v.id) {
			lowerBound = mid + 1;
		} else {
			check = true;
			break;
		}
    }
    
    // If the vertex with the indicated ID is not pointed to by the current vertex, return -1
    if (check == false) {
		mid = -1;
    }

    return mid;
}

INT16 vertex::isBackNeighbor(vID v) {
    bool check = false;	// Equals false if n is not a back neighbor
    INT16 mid;		
    INT16 lowerBound = 0;
    // Set the upper bound to start as the last possible position of the vertex list
    INT16 upperBound = inDegree - 1;
    
    // While the indicated id has not been found and the entire list has not been searched
    while (lowerBound <= upperBound) {
		mid = lowerBound + ((upperBound - lowerBound) / 2);
	
		if ((backNList[mid]).vid.id > v.id) {
			upperBound = mid - 1;
		} else if ((backNList[mid]).vid.id < v.id) {
			lowerBound = mid + 1;
		} else {
			
				check = true;
			
			break;
		}
    }
    
    // If the vertex with the indicated ID is not pointed to by the current vertex, return -1
    if (check == false) {
		mid = -1;
    }

    return mid;
}

bool vertex::addNeighbor0(Edge e) {
	UINT16 j = outDegree;
	for (; j > 0 ; j--) {
		// moves array entries greater than vID n to the right to insert n in order
		if (nList[j-1].vid.id > e.vid.id) {   
			nList[j] = nList[j-1];
		} else {
			break;
		}
	}
	
	nList[j] = e;		// add it to its correct spot
	outDegree++;	
	return true;
}

bool vertex::addNeighbor1(Edge e, INT16 allocation) {
	bool rc = false;
	if (isNeighbor(e.vid) == -1) {		// if the neighbor doesnt already exist.
		if (outDegree == capacity) {	// if the list needs to be expanded
			try {
				Edge *newList = new Edge[capacity + allocation];
				for (int i = 0; i < capacity; i++) {
					newList[i] = nList[i];				// copy array over to the new bigger array
				}
				delete [] nList;						// delete old array.
				nList = newList;
				capacity += allocation;
			} catch (bad_alloc& badAlloc) {
				cerr << "Not enough memory: " << badAlloc.what() << endl;
			}
		}
		rc = addNeighbor0(e);
	}
	return rc;
}


bool vertex::addBackNeighbor0(Edge e) {
	UINT16 j = inDegree;
	for (; j > 0 ; j--) {
		// moves array entries greater than vID n to the right to insert n in order
		if (backNList[j-1].vid.id > e.vid.id) {   
			backNList[j] = backNList[j-1];
		} else {
			break;
		}
	}
	
	backNList[j] = e;			// add it to its correct spot
	inDegree++;			
	return true;
}

bool vertex::addBackNeighbor1(Edge e, INT16 allocation) {
	bool rc = false;
	if (isBackNeighbor(e.vid) == -1) {		// if the neighbor doesnt already exist.
		if (inDegree == backCapacity) {		// if the list needs to be expanded
			try {
				Edge *newList = new Edge[backCapacity + allocation];
				for (int i = 0; i < backCapacity; i++) {		// copy array over to the new bigger array
					newList[i] = backNList[i];
				}
				delete [] backNList;		// delete old array.
				backNList = newList;
				backCapacity += allocation;
			} catch (bad_alloc& badAlloc) {
				cerr << "Not enough memory: " << badAlloc.what() << endl;
			}
		}
		rc = addBackNeighbor0(e);
	}
	return rc;
}

bool vertex::delNeighbor(Edge e) {
    bool rc = false;
    INT16 t = isNeighbor(e.vid);

    if (t != -1) {
		// Create new array with decremented capacity.
		Edge *temp = new Edge[capacity - 1];	
		
		for (int i = 0; i < t; i++) {
			temp[i] = nList[i];
		}

		for (int i = t; i < capacity-1; i++) {
			temp[i] = nList[i + 1];
		}

		

		delete [] nList;
		nList = temp;
	
		capacity--;
		outDegree--;

		rc = true;
	}

    return rc;
}

bool vertex::delBackNeighbor(Edge e) {
    bool rc = false;
    INT16 t = isBackNeighbor(e.vid);

    if (t != -1) {
		// Create new array with decremented capacity.
		Edge *temp = new Edge[backCapacity - 1];	
		
		for (int i = 0; i < t; i++) {
			temp[i] = backNList[i];
		}

		for (int i = t; i < backCapacity-1; i++) {
			temp[i] = backNList[i + 1];
		}

		delete [] backNList;
		backNList = temp;
	
		backCapacity--;
		inDegree--;

		rc = true;
	}

    return rc;
}

void vertex::incInDegree() {
    inDegree++;
}

void vertex::decInDegree() {
    inDegree--; 
}

void vertex::printNeighbors() {
    for (UINT16 j = 0; j < outDegree; j++) {	// print each value in nList
		cout << (nList[j].vid.id) << " ";
    }
    cout << endl;
}

void vertex::printBackNeighbors() {
    for (UINT16 j = 0; j < inDegree; j++) {    	// print each value in nBackList
		cout << (backNList[j].vid.id) << " ";
    }
    cout << endl;
}

void vertex::printNeighborsWeights() {
    for (UINT16 j = 0; j < outDegree; j++) {	// print the weights
		cout << (nList[j].weight) << " ";
    }
    cout << endl;
}

void vertex::incPos() { 	// my position within myself tracking
	myPos++;
}

void vertex::decPos() {
	myPos--;
}

void vertex::incBackPos() {
	myBackPos++;
}

void vertex::decBackPos() {
	myBackPos--;
}

void vertex::setWeight(vID v, UINT16 w) {		// set the weight of an edge
	if (isNeighbor(v) >= 0) {
		nList[isNeighbor(v)].weight = w;
	}
}

void vertex::setBackWeight(vID v, UINT16 w) {
	if (isNeighbor(v) >= 0) {
		backNList[isBackNeighbor(v)].weight = w;
	}
}

UINT16 vertex::getWeight(vID v) {
	return nList[v.id].weight;
}

UINT16 vertex::getBackWeight(vID v) {
	return backNList[v.id].weight;
}

UINT16 vertex::getInDegree() {
    return inDegree;
}

UINT16 vertex::getOutDegree() {
    return outDegree;
}

UINT16 vertex::getCapacity() const {
	return capacity;
}

UINT16 vertex::getBackCapacity() const {
	return backCapacity;
}

Edge * vertex::getNList() const {
	return nList;
}

Edge * vertex::getBackNList() const {
	return backNList;
}

Edge vertex::getEdge(vID v) {	
	return nList[v.id];
}

// -------------------------------------- Graph -----------------------------------------

// Constructor for graph
graph::graph(UINT64 numberOfVertices) {
    vertexCount = numberOfVertices;
    vList = new vertex[vertexCount];
    totalDegree = 0;
	allocation = 1;
}

// Destructor for graph
graph::~graph() {
    clear();
    delete [] vList;
}

void graph::clear() {
    for (UINT64 i = 0; i < vertexCount; i++) {
		vList[i].clearVertex();
		vList[i].clearBackVertex();

    }
    totalDegree = 0;
}

// Clear the neighbor list of a particular vertex
void graph::clearNlist(vID v) {
	Edge * clearList = getNList(v.id);
	// Decrement all of the vertices affected
	for (int i = 0; i < vList[v.id].getOutDegree(); i++) {
		vList[(clearList[i]).vid.id].delBackNeighbor(getEdge(v, 0));
	}
	// Decrease total degree by the appropriate amount
	totalDegree = totalDegree - vList[v.id].getOutDegree();
	vList[v.id].clearVertex();
}

void graph::setAllocation(INT16 a) {
	if (a < 1) {
		allocation = 1;
	} else if (a > 16) {
		allocation = 16;
	} else {
		allocation = a;
	}
}

bool graph::addEdge(vID u, vID v, UINT16 weight, bool inc) {
    bool rc = false;
	Edge e = getEdge(v, weight);
	rc = vList[u.id].addNeighbor1(e, allocation);

	if (rc) {
		// If the edge is successfully added, increase vertex v's inDegree by 1
		vList[v.id].addBackNeighbor1(getEdge(u, weight), allocation);

		totalDegree++;
		if (u.id > v.id) {
			vList[u.id].incPos();
		} else if (v.id > u.id){
			vList[v.id].incBackPos();
		}
	} else {		// if the edge exists change weight
		if (inc) {
			vList[u.id].setWeight(v, ((vList[u.id].getWeight(v)) + weight));
			//set the back edge weight
			vList[v.id].setBackWeight(u, ((vList[v.id].getBackWeight(u)) + weight)); 
		} else {
			vList[u.id].setWeight(v, weight);
			vList[v.id].setBackWeight(u,weight);
		}
	}
    return rc;
}

bool graph::delEdge(vID u, vID v, UINT16 weight) {
    bool rc;
	
    rc = vList[u.id].delNeighbor(getEdge(v, weight));

    if (rc) {
		// If the edge is successfully deleted, decrease vertex v's inDegree by 1
		vList[v.id].delBackNeighbor(getEdge(u,weight));
		totalDegree--;
		if (u.id > v.id) {
			vList[u.id].decPos();
		} else if (v.id > u.id) {
			vList[v.id].decBackPos(); 
		}
		
    }

    return rc;
}

bool graph::isEdge(vID u, vID v, UINT16 weight) {
    bool rc = false;
	Edge e = getEdge(v, weight);
 
    // If vertex u points to vertex v, return true
    if (vList[u.id].isNeighbor(v) != -1) {
		rc = true;
    }

    return rc;
}

bool graph::isBackEdge(vID u, vID v, UINT16 weight) {
	bool rc = false;
 
    // If vertex u points to vertex v, return true
    if (vList[u.id].isBackNeighbor(v) != -1) {
		rc = true;
    }

    return rc;
}

void graph::dumpGraph() {
	Edge *E;
	UINT64 *A;
	int n = 0;
	FILE * pfile;
    pfile = fopen("dumpOutput.txt","w");
	for (UINT64 i = 0; i < vertexCount; i++) {
		E = vList[i].getNList();
		n = vList[i].getOutDegree();
		for (UINT32 j = 0; j < n; j++) {
			fprintf(pfile, "%u %u \n",i,E[j].vid.id);
		}
	}
	
    fclose(pfile);
	
}

void graph::printVneighbors(vID v) {
    // Print out the neighbors for vertex v
    cout << "Vertex " << v.id << "'s neighbors are: ";
    vList[v.id].printNeighbors();
}

void graph::printVBackNeighbors(vID v) {
    // Print out the back neighbors for vertex v
    cout << "Vertex " << v.id << "'s backedge neighbors are: ";
    vList[v.id].printBackNeighbors();
}

void graph::printVneighborsWeights(vID v) {
    // Print out the neighbors for vertex v
    cout << "Vertex " << v.id << "'s weights are: ";
    vList[v.id].printNeighborsWeights();
}

UINT16 graph::getVinDegree(vID v) const {
    return(vList[v.id].getInDegree());
}

UINT16 graph::getVoutDegree(vID v) const {
    return(vList[v.id].getOutDegree());
}

UINT64 graph::getVertexCount() const {
    // Return the number of vertices there are in the entire adjacency list
    return vertexCount;
}

UINT64 graph::getTotalDegree() const {
    // Return |E| - this only works for a directed graph
    return totalDegree;
}

UINT64 graph::getUnusedSpace() const {
	UINT64 space = 0;
	for (int i = 0; i < vertexCount; i++) {
		space += (vList[i].getCapacity() - vList[i].getOutDegree());
	}
	
	return space;
}

Edge * graph::getNList(UINT64 v) {
	return vList[v].getNList();
}

Edge * graph::getBackNList(UINT64 v) {
	return vList[v].getBackNList();
}

Edge graph::getEdge(vID v, UINT16 weight) {
	Edge e;
	e.vid = v;
	e.weight = weight;
	return e;
}

void graph::printGraph() {
    // Print out the ID of each vertex that the indicated vertex points to
    for (UINT64 i = 0; i < vertexCount; i++) {
	cout << "Vertex " << i << "'s neighbors are: ";
	vList[i].printNeighbors();
	cout << "Vertex " << i << "'s weights are:   ";
	vList[i].printNeighborsWeights();
	cout << endl;
    }
}
