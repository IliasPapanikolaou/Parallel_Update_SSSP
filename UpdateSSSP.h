//
// Created by elias on 9/3/2021.
//

#ifndef ERGASIA2_UPDATESSSP_H
#define ERGASIA2_UPDATESSSP_H

#include "Edge.h"

#define V 20
//#define V 20000 //large_graph.txt
#define INF 1000000000

using namespace std;

struct threadArgs{
    int *dist;
    int *parent;
    std::vector<Edge> changedEdges;
    int ThreadId;
};

//Algorithm 2: Updating SSSP for a Single Change
void updatePerChange(vector<Edge> ce, int * Dist, int * Parent);

void processVertex(int u);

//Initialize Matrices Algorithm 3 (lines 2-5)
void *initMatricesPThreads(void *args);
//Initialize Matrices Algorithm 3 (lines 6-29)
void *updateBatchChangePThreads(void *args);

//Algorithm 4 Step 2: Updating Affected Vertices in Parallel
void *processVertexParallel(void *threadid);



#endif //ERGASIA2_UPDATESSSP_H
