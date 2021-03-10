//
// Created by elias on 8/3/2021.
//

#include <stdio.h>
#include <iostream>
#include <deque>
#include <limits.h>
#include "Edge.h"
#include "Dijkstra.h"

deque<int> pq;

int * Dijkstra::getDist(){
    return Dist;
}

int * Dijkstra::getParent(){
    return Parent;
}

//Print Distance and Parent matrices
void Dijkstra::printSolution(int src) {
    printf("---------Dijkstra Algorithm-------------\n");
    printf("Edge Distance from Source (Vector %d):\n", src);
    for (int i = 0; i < V; i++)
        printf("%d distance from %d is %d parent %d\n", i, src, Dist[i], Parent[i]);
}

//Algorithm 1 Paper - Dijkstra’s Algorithm for Finding SSSP
void Dijkstra::dijkstra(int Graph[][V], int src){
    //Initialize Dist, Parent and PriorityQueue PQ.
    for(int u = 0; u < V; u++){
        Dist[u] = INF;
        Parent[u] = u;
    }
    pq.push_back(src);
    Dist[src] = 0;

    while(!pq.empty()){
        int u = pq.front();
        pq.pop_front();
        for(int n = 0; n < V; n++){
            //n where n is neighbor of u
            if(!Graph[u][u] && Graph[u][n] > 0 && Dist[n] > Dist[u] + Graph[u][n]){
                Dist[n] = Dist[u] + Graph[u][n];
                pq.push_back(n);
                Parent[n] = u;
            }
        }
    }

    printSolution(src);
}




