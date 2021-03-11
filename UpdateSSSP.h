//
// Created by elias on 9/3/2021.
//

#ifndef ERGASIA2_UPDATESSSP_H
#define ERGASIA2_UPDATESSSP_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <deque>
#include <limits.h>

#define V 20
#define INF 1000000000
#define NUM_THREADS 4

using namespace std;

class UpdateSSSP {

private:
    int DistUpdated[V];
    int ParentUpdated[V];
    bool Affected[V];
    deque<int> pq;

public:
    //Algorithm 2: Updating SSSP for a Single Change
    void updatePerChange(vector<Edge> ce, int Graph[V][V], int * Dist, int * Parent);

    void processVertex(int u, int Graph[V][V]);

    //Algorithm 3 Step1: Processing Changed Edges in Parallel
    void updateBatchChange(vector<Edge> ce, int Graph[V][V], int * Dist, int * Parent);

    //Algorithm 4 Step 2: Updating Affected Vertices in Parallel
    void processVertexParallel(int Graph[V][V]);

};


#endif //ERGASIA2_UPDATESSSP_H
