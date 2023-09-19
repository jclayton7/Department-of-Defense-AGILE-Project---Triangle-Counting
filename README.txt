// File: README.txt
// Author: Judah Clayton
// Purpose: This file shows the command line commands that
// should be used to generate the graphs and run the
// triangle counting program on them. 
// graph500-scale24-ef16_adj.tsv can be found at
// http://graphchallenge.mit.edu/data-sets but must 
// be unzipped. To change the number of threads used, 
// change THREAD_COUNT in sugraphm11-21-v2 or latticeGen-v3.
// The number of vertices and edges and the seed can be changed
// for the auto-generated graph. When the -v# flag is used, the 
// program will create a graph of size 2^#.

Compiling
g++ tcMT-v4.cpp sugraphm11-21-v2.cpp graph.cpp graphGen.cpp -O2 -fopenmp -o tcMT-v4

Running - Auto-Generated Graph
g++ tcMT-v4.cpp sugraphm11-21-v2.cpp graph.cpp graphGen.cpp -O2 -fopenmp -o tcMT-v4
./tcMT-v4 -g -v28 -e16 -s -c5

Converting and Running - Graph500 Graph
g++ mvconvert.cpp -o convert.exe 
./convert.exe < graph500-scale24-ef16_adj.tsv 
./tcMT-v4 -f /home/jclayton/SenProj/dumpedGraph.bin

Converting and Running - Any External Graph
g++ mvconvert-original.cpp -o convert.exe 
./convert.exe < FILE_NAME
./tcMT-v4 -f /home/jclayton/SenProj/dumpedGraph.bin


Running - Lattice Graph
g++ tcMT-v4.cpp latticeGen-v3.cpp graph.cpp graphGen.cpp -O2 -fopenmp -o tcMT-v4
./tcMT
