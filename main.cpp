#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include "Edge.h"
#include "Dijkstra.h"

using namespace std;

//Number of Vertices
#define V 20 //test_graph.txt
//#define V 4039 //facebook_combined.txt

//Graph
std::vector<Edge> graph;
//Adjacent Graph
int Adj[V][V] = {{0}};
//Changed Edges
std::vector<Edge> ce;
//Dijkstra object
Dijkstra dijkstra;


int readGraphFile(string filename){

    ifstream input;
    input.open(filename);

    if(!input.is_open()){
        cout << "File was not found!" << endl;
        return 1;
    }
    int vertex;
    int neighbour;

    while (input >> vertex >> neighbour){
        //cout << vertex  << " - " << neighbour << " " << endl;
        Edge edge;
        edge.a = vertex;
        edge.b = neighbour;
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

//Add changed edges to ce vector
void addToChangedEdges(Edge edge){
    //int* Dist = dijkstra.getDist();
    ce.push_back(edge);

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
        }
        else{
            Adj[x][y] = 0;
            Adj[y][x] = 0;
            cout << "edge " << x << "-" << y <<" snapped."<<endl;
        }

        addToChangedEdges(edge);
    }
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

    dijkstra.dijkstra(Adj, 0);

    //snap or add random edges on graph
    //define number of random edges to be snapped or added
    addOrSnapRandomEdges(5);
    //Print Adjacent matrix
    printAdjacentMatrix();
    dijkstra.dijkstra(Adj, 0);

    return 0;
}