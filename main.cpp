#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <chrono>
#include <pthread.h>
#include "Edge.h"
#include "Dijkstra.h"
#include "UpdateSSSP.h"

using namespace std;
using namespace std::chrono;

//Number of Vertices
#define V 20 //test_graph.txt
//#define V 20000 //large_graph.txt
#define NUM_THREADS 4

//edges
std::vector<Edge> edges;
//Adjacent Graph
int Graph[V][V] = {{0}};
//Changed Edges
std::vector<Edge> ce; //changed Edges

extern int DistUpdated[V];
extern int ParentUpdated[V];

int readGraphFile(string filename){

    ifstream input;
    input.open(filename);

    if(!input.is_open()){
        cout << "File was not found!" << endl;
        return 1;
    }
    int vertex;
    int neighbor;

    while (input >> vertex >> neighbor){
        //cout << vertex  << " - " << neighbor << " " << endl;
        Edge edge;
        edge.a = vertex;
        edge.b = neighbor;
        edges.push_back(edge);
    }

    input.close();
    return 0;
}

void createAdjMatrix(){

    for(int i = 0; i < edges.size(); i++){
        Edge edge = edges.at(i);
        int x = edge.a;
        int y = edge.b;

        Graph[x][y] = 1;
        Graph[y][x] = 1;
    }
}

void addOrSnapRandomEdges(int numOfEdges){
    cout << "-------Changing some edges-------" << endl;
    //initialize random seed
    srand(time(NULL));
    for(int i = 0; i < numOfEdges; i++){
        //rng within V range
        int x = rand() % V;
        int y = rand() % V;

        Edge edge;
        edge.a = x;
        edge.b = y;

        if(Graph[x][y] == 0){
            Graph[x][y] = 1;
            Graph[y][x] = 1;
            cout << "edge " << x << "-" << y <<" added."<<endl;
            ce.push_back(edge);
        }
        else{
            Graph[x][y] = 0;
            Graph[y][x] = 0;
            cout << "edge " << x << "-" << y <<" snapped."<<endl;
            edge.isPresent = false; //deleted edge
            ce.push_back(edge);
        }
    }
}

void printAdjacentMatrix(){
    cout << "------------Adjacent matrix------------"<< endl;
    for(int i = 0; i < V; i++){
        for(int j = 0; j < V; j++){
            cout << Graph[i][j] << " ";
        }
        cout << endl;
    }
}


int main () {

    string filename = "test_graph.txt";
    //string filename = "large_graph.txt";
    readGraphFile(filename);
    createAdjMatrix();

    //Print Adjacent matrix
    printAdjacentMatrix();

    //Run Dijkstra algorithm on Adjacent matrix
    Dijkstra dijkstra;
    dijkstra.dijkstra(Graph, 0);

    //snap or add random edges on edges
    //define number of random edges to be snapped or added
    addOrSnapRandomEdges(5);

    //Print Updated Adjacent matrix
    //printAdjacentMatrix();
    //Dijkstra 2nd round
    //dijkstra.dijkstra(Graph, 0);

    //Timer Start Sequential
    auto startSequential = high_resolution_clock::now();
    //Algorithm 2: Updating SSSP for a Single Change (Sequential Algorithm)
    updatePerChange(ce, dijkstra.getDist(), dijkstra.getParent());
    auto stopSequential = high_resolution_clock::now();
    auto durationSequential = duration_cast<microseconds>(stopSequential - startSequential);

    //Timer Start Parallel
    auto startParallel = high_resolution_clock::now();
    //Parallel Update with Pthreads
    pthread_t threads[NUM_THREADS];
    struct threadArgs args[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++){
        args[i].changedEdges = ce;
        args[i].dist = dijkstra.getDist();
        args[i].parent = dijkstra.getParent();
    }
    //Algorithm 3: Step 1 Processing Changed Edges in Parallel (Pthreads)
    //Initialize Matrices Algorithm 3 (lines 2-5)
    for(int i =  0; i < NUM_THREADS; i++){
        args[i].ThreadId = i;
        pthread_create(&threads[i], NULL, initMatricesPThreads, (void*)&args[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    for(int i =  0; i < NUM_THREADS; i++){
        args[i].ThreadId = i;
        pthread_create(&threads[i], NULL, updateBatchChangePThreads, (void*)&args[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    //Algorithm 4: Step 2: Updating Affected Vertices in Parallel (PThreads)
    for(int i =  0; i < NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, processVertexParallel, (void *)i);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    //Timer Stop Parallel
    auto stopParallel = high_resolution_clock::now();
    auto durationParallel = duration_cast<microseconds>(stopParallel - startParallel);

    cout << "---------Parallel Updated SSSP-------------" << endl;
    for (int i = 0; i < V; i++)
        cout << i << " distance is " << DistUpdated[i] << " parent " << ParentUpdated[i] << endl;

    cout << endl << "------Timer for Sequential Updated SSSP------" << endl;
    cout << "Time taken: "<< durationSequential.count() << " microseconds" << endl;
    cout << endl << "------Timer for Parallel Updated SSSP--------" << endl;
    cout << "Time taken: "<< durationParallel.count() << " microseconds" << endl;
    return 0;
}