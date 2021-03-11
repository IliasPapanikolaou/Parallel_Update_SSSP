#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <pthread.h>
#include "Edge.h"
#include "Dijkstra.h"
#include "UpdateSSSP.h"

using namespace std;

//Number of Vertices
#define V 20 //test_graph.txt
//#define V 4039 //facebook_combined.txt
#define NUM_THREADS 4

//Graph
std::vector<Edge> graph;
//Adjacent Graph
int Adj[V][V] = {{0}};
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
        graph.push_back(edge);
    }

    input.close();
    return 0;
}

void createAdjMatrix(){

    for(int i = 0; i < graph.size(); i++){
        Edge edge = graph.at(i);
        int x = edge.a;
        int y = edge.b;

        Adj[x][y] = 1;
        Adj[y][x] = 1;
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

        if(Adj[x][y]==0){
            Adj[x][y] = 1;
            Adj[y][x] = 1;
            cout << "edge " << x << "-" << y <<" added."<<endl;
            ce.push_back(edge);
        }
        else{
            Adj[x][y] = 0;
            Adj[y][x] = 0;
            cout << "edge " << x << "-" << y <<" snapped."<<endl;
            edge.isPresent = false; //deleted edge
            ce.push_back(edge);
        }
    }
    copy(&Adj[0][0], &Adj[V][V], &Graph[0][0]);
}

void printAdjacentMatrix(){
    cout << "------------Adjacent matrix------------"<< endl;
    for(int i = 0; i < V; i++){
        for(int j = 0; j < V; j++){
            cout << Adj[i][j] << " ";
        }
        cout << endl;
    }
}


int main () {

    string filename = "test_graph.txt";
    //string filename = "facebook_combined.txt";
    readGraphFile(filename);
    createAdjMatrix();

    //Print Adjacent matrix
    printAdjacentMatrix();

    //Run Dijkstra algorithm on Adjacent matrix
    Dijkstra dijkstra;
    dijkstra.dijkstra(Adj, 0);

    //snap or add random edges on graph
    //define number of random edges to be snapped or added
    addOrSnapRandomEdges(3);

    //Print Updated Adjacent matrix
    //printAdjacentMatrix();
    //Dijkstra 2nd round
    //dijkstra.dijkstra(Adj, 0);

    //Algorithm 2: Updating SSSP for a Single Change (Sequential Algorithm)
    updatePerChange(ce, dijkstra.getDist(), dijkstra.getParent());

    updateBatchChange(ce, dijkstra.getDist(), dijkstra.getParent());

    //Parallel Update with Pthreads :-(
    pthread_t threads[NUM_THREADS];


    //Algorithm 4 with PThreads
    for(int i =  0; i < NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, processVertexParallel, (void *)i);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    cout << "---------Parallel Updated SSSP-------------" << endl;
    for (int i = 0; i < V; i++)
        cout << i << " distance is " << DistUpdated[i] << " parent " << ParentUpdated[i] << endl;


    return 0;
}