// File: tc_graph.h
// Author: Judah Clayton
// Date: 10/24/2022

int TC(double *array, UINT32 *array2); // Calculate the number of trust and cycle triangles.

UINT32 cycle(UINT64 &tec); // Calculate the number of cycle triangles.

UINT64 cycleLoop(UINT64 i, UINT64 &tec); // This is the body of the outer for loop.

UINT32 trust(UINT64 &tec); // Calculate the number of trust triangles.

UINT64 trustLoop(UINT64 i, UINT64 &tec); // This is the body of the outer for loop.

UINT32 intersectCard(Edge *nList1, Edge *nList2, UINT16 list1LB, UINT16 list1UB, 
					 UINT16 list2LB, UINT16 list2UB);