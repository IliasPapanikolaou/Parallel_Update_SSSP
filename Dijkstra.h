#ifndef ERGASIA2_DIJKSTRA_H
#define ERGASIA2_DIJKSTRA_H

#include <limits.h>
#include <stdio.h>
#include <iostream>
#include <vector>

// Number of vertices in the graph
#define V 20 //test_graph
// #define V 4039 //facebook_combined.txt

using namespace std;

class Dijkstra {

private:
    int Dist[V];
    int Parent[V];

public: int * getDist(){
        return Dist;
    }

public: int * getParent(){
        return Parent;
    }

    // A utility function to find the vertex with minimum distance value, from
    // the set of vertices not yet included in shortest path tree
    int minDistance(int dist[], bool sptSet[])
    {
        // Initialize min value
        int min = INT_MAX, min_index;

        for (int v = 0; v < V; v++)
            if (sptSet[v] == false && dist[v] <= min)
                min = dist[v], min_index = v;

        return min_index;
    }

    // A utility function to print the constructed distance array
    int printSolution(int dist[], int parent[], int src)
    {
        printf("---------Dijkstra Algorithm-------------\n");
        printf("Edge Distance from Source (Vector %d):\n", src);
        for (int i = 0; i < V; i++)
            printf("%d distance from %d is %d parent %d\n", i, src, dist[i], parent[i]);
    }

    // Function that implements Dijkstra's single source shortest path algorithm
    // for a graph represented using adjacency matrix representation
public: void dijkstra(int graph[V][V], int src){

        bool sptSet[V]; // sptSet[i] will be true if vertex i is included in shortest
        // path tree or shortest distance from src to i is finalized

        // Initialize all distances as INFINITE and stpSet[] as false
        for (int i = 0; i < V; i++){
            Dist[i] = INT_MAX, sptSet[i] = false;
            Parent[i] = i;
        }

        // Distance of source vertex from itself is always 0
        Dist[src] = 0;

        // Find shortest path for all vertices
        for (int count = 0; count < V - 1; count++) {
            // Pick the minimum distance vertex from the set of vertices not
            // yet processed. u is always equal to src in the first iteration.
            int u = minDistance(Dist, sptSet);

            // Mark the picked vertex as processed
            sptSet[u] = true;

            // Update dist value of the adjacent vertices of the picked vertex.
            for (int v = 0; v < V; v++)

                // Update dist[v] only if is not in sptSet, there is an edge from
                // u to v, and total weight of path from src to  v through u is
                // smaller than current value of dist[v]
                if (!sptSet[v] && graph[u][v] && Dist[u] != INT_MAX && Dist[u] + graph[u][v] < Dist[v]){
                    Dist[v] = Dist[u] + graph[u][v];
                    Parent[v] = u;
                }
        }

        // print the constructed distance array
        printSolution(Dist, Parent, src);
    }

};


#endif //ERGASIA2_DIJKSTRA_H
