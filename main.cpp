#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include "Vertex.h"
#include "Dijkstra.h"

using namespace std;

//Number of Vertices
#define V 20 //test_graph.txt
//#define V 4039 //facebook_combined.txt

std::vector<Vertex> graph;
static int Adj[V][V] = {{false}};


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
        Vertex v;
        v.vertex_id = vertex;
        v.neighbour_id = neighbour;
        graph.push_back(v);
    }

    input.close();
    return 0;
}

void createAdjMatrix(){

    for(int i = 0; i < graph.size(); i++){
        Vertex v = graph.at(i);
        int x = v.vertex_id;
        int y = v.neighbour_id;

        Adj[x][y] = 1;
        Adj[y][x] = 1;
    }
}

int main () {

    string filename = "test_graph.txt";
    //string filename = "facebook_combined.txt";
    readGraphFile(filename);
    createAdjMatrix();

    //Print Adjacent matrix
    cout << "Adjacent matrix: "<< endl;
    for(int i = 0; i < V; i++){
        for(int j = 0; j < V; j++){
            cout << Adj[i][j] << " ";
        }
        cout << endl;
    }

    Dijkstra dijkstra;
    dijkstra.dijkstra(Adj, 0);

    return 0;
}