#ifndef ERGASIA2_DIJKSTRA_H
#define ERGASIA2_DIJKSTRA_H

// Number of vertices in the edges
#define V 20 //test_graph
#define INF 1000000000

using namespace std;

class Dijkstra{

private:
    int Dist[V];
    int Parent[V];

public:
    int * getDist();
    int * getParent();

//Algorithm 1 Paper - Dijkstra’s Algorithm for Finding SSSP
    void dijkstra(int Graph[][V], int src);

//Print Distance and Parent matrices
    void printSolution(int src);
};

#endif //ERGASIA2_DIJKSTRA_H
