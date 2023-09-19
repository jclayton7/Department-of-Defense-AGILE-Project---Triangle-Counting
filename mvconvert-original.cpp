// File: mvconvert.cpp
// Author: Michael VanDusen
// Date: 3/1/22
// Purpose: To convert The Biovista graph to a binary file

#include <cstdio>
#include <iostream>			// Provides cout
#include <time.h>

using namespace std;

int main() {
    // Create a graph with 32000000 vertices
    clock_t clock0 = clock();//Start of the program
    unsigned int *A = new unsigned int[300];
    clock_t clock1 = clock();//Array has been assigned
   
    cout << "Start of Read" << endl;
    long long n = 0;
    while(cin >> A[n] >> A[n+1]){
        n = n + 2;
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
