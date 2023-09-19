// File: mvconvert-v1.cpp
// Authors: Michael VanDusen
// Edits made by: Judah Clayton
// Date: 3/1/22
// Purpose: To convert the scale-24 Graph500 file 
// (graph500-scale24-ef16_adj.tsv) graph to a binary file

#include <cstdint>          // Provides type uint8_t. Addition made by Judah Clayton.
#include <cstdio>
#include <iostream>			// Provides cout
#include <time.h>

using namespace std;

int main() {
    // Create a graph with 32000000 vertices
    clock_t clock0 = clock();//Start of the program
    unsigned int *A = new unsigned int[520523686 * 3]; // The array needs to hold three 
                                             // values for each line of the graph500 
                                             // file. Edit made by Judah Clayton 
                                             // (original was "unsigned int 
                                             // *A = new unsigned int[300];").
    clock_t clock1 = clock();// Array has been assigned
    
    int i = 0; // Addition made by Judah Clayton.
        
    cout << "Start of Read" << endl;
    long long n = 0;
    while(cin >> A[n] >> A[n+1] >> A[n+2]){ // Edit made by Judah Clayton.
        n = n + 3; // Edit made by Judah Clayton.
        i++;
    }
    cout << "Number of pairs " << n/2 << endl;
    cout << "End of Read" << endl;
    cout << endl;
    clock_t clock2 = clock();//Graph has been populated
    FILE * pfile;
    pfile = fopen("dumpedGraph.bin","wb");
    fwrite(A, 4, n, pfile);
    fclose(pfile);
    clock_t clock3 = clock();//Conversion to the binary file
    float ttime = (float)(clock1 - clock0) / (float)CLOCKS_PER_SEC;
    //ttime = ttime / 1000.0; 
    cout << "\n\nThe time taken to allocate the array: " << ttime <<endl;
    ttime = (float)(clock2 - clock1) / (float)CLOCKS_PER_SEC;
    //ttime = ttime / 1000.0; 
    cout << "The time taken to read in the file: " << ttime <<endl;
    ttime = (float)(clock3 - clock2) / (float)CLOCKS_PER_SEC;
    cout << "The time taken to convert to the binary file: " << ttime <<endl;
    
    return 0;
}

