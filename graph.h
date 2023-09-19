// File: graph.h
// Author: Dr. Stephen Wheat, Austin Lehman, Samuel Udall, Michael Vandusen, Andrew Westlund
// Date: 1/31/22
// Pupose: This program will recieve a graph and will create a list of all the vertices
// and will list all the vertices adjacent to each vertex.
// The both the list and the lists of adjacent vertices will be dynamically allocated
// and will be sorted least to greatest. 

#ifndef __GRAPH_H
#define __GRAPH_H

// Create typedefs for variables
typedef unsigned long long UINT64;
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;

typedef long long INT64;
typedef int INT32;
typedef short INT16;
typedef char INT8;

class vID {
    public:
	UINT64 id;
};

class Edge {
	public:
	vID vid;
	UINT16 weight;
	UINT16 pad1;	// padding to round out memory to multiples of 8
	UINT32 pad2;
};

class vertex {
    private:
	Edge *nList;	// An array of edge objects (neighbor list for a given index).
	Edge *backNList;		// An array of vertex ID objects (outgoing neighbor list for a given index).
	UINT16 inDegree;	// The count of incoming egdes.
	UINT16 outDegree;	// The count of outgoing egdes.
	UINT16 capacity;	// The capacity of the vertex array.
	UINT16 backCapacity; // The capacity of the back edge array.
	UINT16 myPos;		// Identifies where this vertex should be in the nList if it were there.
	UINT16 myBackPos;	// Identifies where this vertex should be in the nBackList if it were there.
	UINT32 pad;			// padding to round out memory to multiples of 8
	
	// Preconditions: vID v is not already on the adjacency list
	// Postconditions: Will insert the vID to the adjacency list in ascending order
	bool addNeighbor0(Edge e);
	
	// Preconditions: vID v is not already on the backedge neighbor list
	// Postconditions: Will insert the vID to the backedge neighbor list in ascending order
	bool addBackNeighbor0(Edge e);
	
    public:
	// Constructor of an object to hold an array of vertices and related variables
	vertex();
	~vertex();

	// Preconditions: None.
	// Postconditions: Initializes the nList, setting all values to zero or null.
	void initVertex();

	// Preconditions: None.
	// Postconditions: Initializes the backNList, setting all related values to zero or null.
	void initBackVertex();
		
	// Preconditions: None.
	// Postconditions: Clears the nList and resets the stored values.
	void clearVertex();
	
	// Preconditions: None.
	// Postconditions: Clears the backNList and resets the related stored values.
	void clearBackVertex();
	
	// Preconditions: none
	// Postconditions: increments myPos 
	void incPos();

	// Preconditions: none
	// Postconditions: increments myBackPos 
	void incBackPos();

	// Preconditions: none
	// Postconditions: decrements myBackPos
	void decBackPos();

	
	// Preconditions: none
	// Postconditions: decrements myPos
	void decPos();

	// Preconditions: None. 
	// Postconditions: Will return the position of the indicated vertex if it is in this vertex's 
	// adjacency list; will return -1 if it is not in the vertex's adjacency list.
	INT16 isNeighbor(vID v);
	
	// Preconditions: None. 
	// Postconditions: Will return the position of the indicated vertex if it is in this vertex's 
	// backedge neighbor list(backNList); will return -1 if it is not in the vertex's backedge neighbor list.
	INT16 isBackNeighbor(vID v);
	
	// Preconditions: None
	// Postconditions: Will check if the vertex is already on the vertex's adjacency list;
	// if the vertex is not already on it, it then checks if there is free space on the list, if there is no free space,
	// it will increase the capacity by two, then it calls addNeighbor0 to add the vID.
	bool addNeighbor1(Edge e, INT16 a);
	
	// Preconditions: None
	// Postconditions: Will check if the vertex is already on the vertex's backedge neighbor list;
	// if the vertex is not already on it, it then checks if there is free space on the list, if there is no free space,
	// it will increase the capacity by the indicated size, then it calls addBackNeighbor0 to add the vID.
	bool addBackNeighbor1(Edge e, INT16 a);
	
	// Preconditions: None
	// Postconditions: Will return true if the vertex is on the vertex's adjacency list and then 
	// removes it; if it is not on the list, will return false.
	bool delNeighbor(Edge e);
	
	// Preconditions: None
	// Postconditions: Will return true if the vertex is on the vertex's backedge neighbor list and then 
	// removes it; if it is not on the list, will return false.
	bool delBackNeighbor(Edge e);
	
	// Preconditions: None.
	// Postconditions: Will increase the value of the indegree of the vertex getting pointed to by one.
	void incInDegree();
	
	// Preconditions: None.
	// Postconditions: Will decrease the value of the indegree of the vertex getting pointed to by one.
	void decInDegree();
	
	// Preconditions: None.
	// Postconditions: Will print the list of adjacent vertices.
	void printNeighbors();
	
	// Preconditions: None.
	// Postconditions: Will print the list of backedge-adjacent vertices.
	void printBackNeighbors();
	
	// Preconditions: None.
	// Postconditions: Will print the list of adjacent vertices weights.
	void printNeighborsWeights();
	
	void setWeight(vID v, UINT16 weight);
	
	void setBackWeight(vID v, UINT16 weight);
	
	UINT16 getWeight(vID v);
	
	UINT16 getBackWeight(vID v);
	
	// Preconditions: None
	// Postconditions: Returns the vertex's in degree
	UINT16 getInDegree();

	// Preconditions: None
	// Postconditions: Returns the vertex's out degree
	// return -1.
	UINT16 getOutDegree();

    // Preconditions: None
    // Postconditions: Returns the capacity of  the nList
    UINT16 getCapacity() const;
	
	// Preconditions: None
	// Postconditions: Returns the capacity of the backNList
	UINT16 getBackCapacity() const;
	
	// Preconditions: None
	// Postconditions: Returns the nList
	Edge * getNList() const;
	
	// Preconditions: None
	// Postconditions: Returns the backNList
	Edge * getBackNList() const;
	
	Edge getEdge(vID v);	

    #include "tc_vertex.h"
};

class graph {
    private:
	vertex *vList;		// An array of vertex objects.
	UINT64 vertexCount;	// Stores how many vertices there are in the graph
	UINT64 totalDegree;	// Stores how many edges there are in the graph
	INT16 allocation;   // The value of how much the vertex capacity increments by
	UINT16 pad1;		// padding to round out memory to multiples of 8
	UINT32 pad2;
	
	Edge getEdge(vID v, UINT16 weight);
    
	public:
	// Constructor
	graph(UINT64 numberOfVertices);
	
	// Destructor
	~graph();

	// Preconditions: None.
	// Postconditions: clears the graph and resets all values of all nLists.
	void clear();
	
	// Preconditions: None.
	// Postconditions: Clears the nList and resets the stored values of vID v.
    void clearNlist(vID v);
	
	// Preconditions: None.
	// Postconditions: Will set allocation to a if a is between 1 and 16; if under 1, will be set to 1
	// and if over 16 will be set to 16
	void setAllocation(INT16 a);

	// Preconditions: vertex(u) and vertex(v) must be existing vertices
	// Postconditions: If the edge does not already exist, a new edge is added that 
	// points from vertex(u) to v(v), the edge array for vertex(u) grows, the corresponding 
	// values, i.e. inDegree, are incremented accordingly, and true is returned.
	// Otherwise, return false.
	bool addEdge(vID u, vID v, UINT16 weight = 0, bool inc = false);
	
	// Preconditions: v(u) and v(v) must be existing vertices
	// Postconditions: If the edge already exists, the edge that points from vertex(u) to 
	// vertex(v) is removed, the edge array for vertex(u) shrinks, the corresponding values, 
	// i.e. inDegree, are decremented accordingly, and true is returned.
	// Otherwise, return false.
	bool delEdge(vID u, vID v, UINT16 weight = 0);
	
	// Preconditions: None. 
	// Postconditions: Will return the position of the indicated vertex if it is in this vertex's 
	// adjacency list; will return -1 if it is not in the vertex's adjacency list.
	bool isEdge(vID u, vID v, UINT16 weight = 0);
	
	// Preconditions: vertex(u) and vertex(v) must be existing vertices on the graph.
	// Postconditions: If there is a backedge going from vertex(u) to vertex(v), return true; otherwise, return false.
	bool isBackEdge(vID u, vID v, UINT16 weight = 0);
	
	
	void dumpGraph();
	
	// Preconditions: vNum must be the id of a vertex on the graph.
	// Postconditions: The vertex's neighbors are printed.
	void printVneighbors(vID v);
	
	// Preconditions: v must have an id of a vertex on the graph.
	// Postconditions: The vertex's backedge neighbors are printed.
	void printVBackNeighbors(vID v);
	
	// Preconditions: vNum must be the id of a vertex on the graph.
	// Postconditions: The vertex's neighbors weights are printed.
	void printVneighborsWeights(vID v);
	
	// Preconditions: None.
	// Postconditions: Returns the vertex indegree
	UINT16 getVinDegree(vID v) const;

	// Preconditions: None.
	// Postconditions: Returns the vertex outdegree
	UINT16 getVoutDegree(vID v) const;

	// Preconditions: None
	// Postconditions: Returns |V|
	UINT64 getVertexCount() const;

	// Preconditions: None
	// Postconditions: Returns |E|
	UINT64 getTotalDegree() const;
	
	UINT64 getUnusedSpace() const;

	// Preconditions: v must be the id of a vertex on the graph.
	// Postconditions: The neighbor list of vertex(v) is returned	
	Edge * getNList(UINT64 v);
	
	// Preconditions: v must be the id of a vertex on the graph.
	// Postconditions: The backedge neighbor list of vertex(v) is returned
	Edge * getBackNList(UINT64 v);

	
	// Preconditions: None.
	// Postconditions: Will print the list of all vertices and their adjacent vertices.
	void printGraph();
 
    #include "tc_graph.h"
};
#endif
