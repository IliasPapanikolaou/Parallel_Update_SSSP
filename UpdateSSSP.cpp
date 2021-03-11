//
// Created by elias on 9/3/2021.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <deque>
#include <thread>
#include <limits.h>
#include "UpdateSSSP.h"
#include "Edge.h"

#define V 20
#define INF 1000000000
#define NUM_THREADS 4

using namespace std;


int DistUpdated[V];
int ParentUpdated[V];
extern int Graph[V][V];
bool Affected[V];
deque<int> PQ;


//Algorithm 2: Updating SSSP for a Single Change
void updatePerChange(vector<Edge> ce, int * Dist, int * Parent){
    //Initialize Updated Distance and Parent
    //Copy Dist and Parent matrices
    copy(&Dist[0],&Dist[V], &DistUpdated[0]);
    copy(&Parent[0],&Parent[V], &ParentUpdated[0]);

    //Find the affected vertices
    for(int i = 0; i < ce.size(); i++){
        Edge edge = ce.at(i);
        int x,y;
        if (Dist[edge.a] > Dist[edge.b]){
            x = edge.a;
            y = edge.b;

        }
        else{
            x = edge.b;
            y = edge.a;
        }
        PQ.push_back(x);
        //update DistUpdated
        //if snapped edge
        if(!edge.isPresent) DistUpdated[x] = INF; //Make Affected Vector Distance INFINITE
            //if is added edge
        else {
            DistUpdated[x] = DistUpdated[y] + Graph[edge.a][edge.b];
            ParentUpdated[x] = y; //That was missing from Paper - Algorithm2
        }
    }

    cout << "--------Affected Vertices---------" << endl;
    for(int i = 0; i < PQ.size(); i++){
        int v = PQ.at(i);
        cout << "affected vertex: " << v << " new Distance: " << DistUpdated[v] << endl;
    }

    //Update the subgraph affected by x
    while(!PQ.empty()){
        int u = PQ.front();
        PQ.pop_front();
        //call method processVertex
        processVertex(u);
    }

    cout << "---------Updated SSSP-------------" << endl;
    for (int i = 0; i < V; i++)
        cout << i << " distance is " << DistUpdated[i] << " parent " << ParentUpdated[i] << endl;
}

void processVertex(int u){
    //Find neighbors of the affected vertices
    for(int n=0; n < V; n++){
        //if n where n is neighbor of u
        if(!Graph[u][u] && Graph[u][n] > 0){
            if(DistUpdated[u] == INF && ParentUpdated[n] == u){
                DistUpdated[n] = INF;
                PQ.push_back(n);
            }
            else{
                if(DistUpdated[n] > DistUpdated[u] + Graph[u][n]){
                    DistUpdated[n] = DistUpdated[u] + Graph[u][n];
                    PQ.push_back(n);
                    ParentUpdated[n] = u;
                }
                else if(DistUpdated[u] > DistUpdated[n] + Graph[u][n]){
                    DistUpdated[u] = DistUpdated[n] + Graph[u][n];
                    PQ.push_back(u);
                    ParentUpdated[u] = n;
                }
            }
        }
    }
}

//Algorithm 3 Step1: Processing Changed Edges in Parallel
void updateBatchChange(vector<Edge> ce, int * Dist, int * Parent){
    //TODO: Parallel
    //Initialize Updated Distance and Parent
    for(int u = 0; u < V; u++){
        DistUpdated[u] = Dist[u];
        ParentUpdated[u] = Parent[u];
        Affected[u] = false;
    }
    //TODO: Parallel
    //Find the affected vertex, x
    for(int i = 0; i < ce.size(); i++){
        Edge edge = ce.at(i);
        int x,y;
        if (Dist[edge.a] > Dist[edge.b]){
            x = edge.a;
            y = edge.b;
        }
        else{
            x = edge.b;
            y = edge.a;
        }
        //update DistUpdated
        //If E inserted, update Dist and P arent
        if(edge.isPresent){
            if(DistUpdated[x] > DistUpdated[y] + Graph[edge.a][edge.b]){
                DistUpdated[x] = DistUpdated[y] + Graph[edge.a][edge.b];
                ParentUpdated[x] = y;
                //Mark E as inserted to SSSP Tree
                Affected[x] = true;
            }
        }
            //if E is to be removed
        else {
            //Check if Edge is in SSSP tree
            if(Parent[edge.a] == edge.b || Parent[edge.b] == edge.a){
                DistUpdated[x] = INF;
                Affected[x] = true;
            }
        }
    }
    //Lowest Value Edge gets Inserted
    bool change = true;
    while (change){
        change = false;
        //TODO: Parallel
        for(int i = 0; i < ce.size(); i++){
            Edge edge = ce.at(i);
            //if E marked to be inserted to SSSP
            if(edge.isPresent){
                //Find the affected vertex, x
                int x,y;
                if (Dist[edge.a] > Dist[edge.b]){
                    x = edge.a;
                    y = edge.b;
                }
                else{
                    x = edge.b;
                    y = edge.a;
                }
                //Check replaces higher weighted edge
                if(DistUpdated[x] > DistUpdated[y] + Graph[edge.a][edge.b]){
                    DistUpdated[x] = DistUpdated[y] + Graph[edge.a][edge.b];
                    Parent[x] = edge.b;
                    change = true;
                    Affected[x] = true;
                }
            }
        }
    }
}

//Algorithm 4: Step 2: Updating Affected Vertices in Parallel
void *processVertexParallel(void *threadId){
    //PThreads impl
    int start, end, numOfElements, id = (long )threadId;

    numOfElements = V / NUM_THREADS;
    start = NUM_THREADS * id;
    //end condition for not 0 mod divisions
    if (id != NUM_THREADS - 1) {
        end = start + numOfElements;
    } else {
        end = V;
    }

    //PThreads impl end
    bool change = true;
    while(change){
        change = false;
        //Start - End instead of 0 to V
        for(int u = start; u < end; u++){
            if (!Affected[u]){
                Affected[u] = false;
            }
            for(int n = 0; n < V; n++){
                //n where n is neighbor of u in Graph
                if(!Graph[u][u] && Graph[u][n] > 0){
                    if(DistUpdated[u] == INF && ParentUpdated[n] == u){
                        DistUpdated[n] = INF;
                        Affected[n] = true;
                    }
                    else{
                        if(DistUpdated[n] > DistUpdated[u] + Graph[u][n]){
                            DistUpdated[n] = DistUpdated[u] + Graph[u][n];
                            Affected[n] = true;
                            ParentUpdated[n] = u;
                        }
                        else{
                            if(DistUpdated[u] > DistUpdated[n] + Graph[u][n]){
                                DistUpdated[u] = DistUpdated[n] + Graph[u][n];
                                Affected[u] = true;
                                ParentUpdated[u] = n;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}