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
#include "Edge.h"

#define V 20
#define INF 1000000000

using namespace std;

class UpdateSSSP {

    int DistUpdated[V];
    int ParentUpdated[V];
    deque<int> affected;
    deque<int> pq;

    //Algorithm 2: Updating SSSP for a Single Change
public: void updatePerChange(vector<Edge> ce, int Graph[V][V], int * Dist, int * Parent){
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
            pq.push_back(x);
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
        for(int i = 0; i < pq.size(); i++){
            int v = pq.at(i);
            cout << "affected vertex: " << v << " new Distance: " << DistUpdated[v] << endl;
        }

        //Update the subgraph affected by x
        while(!pq.empty()){
            int u = pq.front();
            pq.pop_front();
            //call method processVertex
            processVertex(u, Graph);
        }

        cout << "---------Updated SSSP-------------" << endl;
        for (int i = 0; i < V; i++)
            cout << i << " distance is " << DistUpdated[i] << " parent " << ParentUpdated[i] << endl;
    }

    void processVertex(int u, int Graph[V][V]){
        //Find neighbors of the affected vertices
        for(int n=0; n < V; n++){
            //if neighbor of u vertex
            if(Graph[u][n] != u && Graph[u][n] > 0){
                if(DistUpdated[u] == INF && ParentUpdated[n] == u){
                    DistUpdated[n] = INF;
                    pq.push_back(n);
                }
                else{
                    if(DistUpdated[n] > DistUpdated[u] + Graph[u][n]){
                        DistUpdated[n] = DistUpdated[u] + Graph[u][n];
                        pq.push_back(n);
                        ParentUpdated[n] = u;
                    }
                    else if(DistUpdated[u] > DistUpdated[n] + Graph[u][n]){
                        DistUpdated[u] = DistUpdated[n] + Graph[u][n];
                        pq.push_back(u);
                        ParentUpdated[u] = n;
                    }
                }
            }
        }
    }
};


#endif //ERGASIA2_UPDATESSSP_H
