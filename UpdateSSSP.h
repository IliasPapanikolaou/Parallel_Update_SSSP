//
// Created by elias on 9/3/2021.
//

#ifndef ERGASIA2_UPDATESSSP_H
#define ERGASIA2_UPDATESSSP_H

#include "Edge.h"

#define V 20
#define INF 1000000000

using namespace std;

//Algorithm 2: Updating SSSP for a Single Change
void updatePerChange(vector<Edge> ce, int * Dist, int * Parent);

void processVertex(int u);

//Algorithm 3 Step1: Processing Changed Edges in Parallel
void updateBatchChange(vector<Edge> ce, int * Dist, int * Parent);

//Algorithm 4 Step 2: Updating Affected Vertices in Parallel
void *processVertexParallel(void *threadid);

#endif //ERGASIA2_UPDATESSSP_H
