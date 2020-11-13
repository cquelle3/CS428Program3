#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <tuple>
#include <sstream>
#include <limits.h>
#include <iomanip>

using namespace std;

int minimum_dist(tuple<int, string> distance[], string N, string nodes, int num_nodes){
    
    //initialize minimum value
    int min = INT_MAX;
    
    //index of the min value in distance array
    int index;
    
    //go through all of the nodes
    for(int i=0; i < num_nodes; i++){
        //if the distance at i is less than the current min
        //and if the node being looked at hasn't been processed
        //update min and index
        if(get<0>(distance[i]) <= min && int(N.find(nodes[i])) < 0){
            min = get<0>(distance[i]);
            index = i;
        }    
    }
    
    //return index which should be the next shortest path available
    return index;
}

int main(int argc, char *argv[]){

    ifstream infile(argv[1]);
    
    string line;
    getline(infile, line);
    int num_nodes = line.length();
    string nodes = line;
    
    //initialize adjacency map
    int adj_map[num_nodes][num_nodes];
    for(int i=0; i < int(nodes.length()); i++){
        for(int j=0; j < int(nodes.length()); j++){
            adj_map[i][j] = INT_MAX;
        }
    }

    
    //read from graph file
    //put in adjacency values for each node
    string s1;
    string s2;
    string s3;
    int current;
    int adj;
    int edge;
    while(getline(infile, line)){
        stringstream s(line);
        s >> s1 >> s2 >> s3;
        
        if(s1 == "adj"){
            adj = nodes.find(s2);
            edge = stoi(s3);
            adj_map[current][adj] = edge;
        }
        else{
            current = nodes.find(s1);
        }
    }

    
    //user enters node that they want the shortest paths to
    string node;
    while(true){
        cout << "Enter a node from the graph: " << endl;
        cin >> node;
        if(int(nodes.find(node)) >= 0){
		break;
        }
        else{
            cout << "Node entered is not in current graph!" << endl;
        }
    }
    
    //initialize array for shortest paths
    tuple<int, string> D[num_nodes][num_nodes];
    for(int i=0; i < int(nodes.length()); i++){
        for(int j=0; j < int(nodes.length()); j++){
            get<0>(D[i][j]) = INT_MAX;
            get<1>(D[i][j]) = "_";
        }
    }
    
    //N string for keeping track of which vertices have been processed
    string N = "";
    
    //keeping track of N as nodes are added to it
    vector<string> V;
    
    //array for keeping track of distances
    tuple<int, string> distance[num_nodes];
    for(int i=0; i < num_nodes; i++){
        get<0>(distance[i]) = INT_MAX;
        get<1>(distance[i]) = "_";
    }

    //initialize the node chosen with a distance of 0 since it cannot be away from itself
    get<0>(distance[nodes.find(node)]) = 0;
    get<1>(distance[nodes.find(node)]) = "_";

    
    //dijkstras algorithm
    for(int i=0; i < num_nodes; i++){
        
        //calculate which node we should go to next based off of which node has the smallest edge
        int next = minimum_dist(distance, N, nodes, num_nodes);
        
        //add the next node to the N string to mark it as processed
        N += nodes[next];
        
        V.push_back(N);
       
        //go through every node
        //if we have not explored this node already, if there is an edge between the two nodes, and if the current total weight is less than the current shortest path, we update the path 
        for(int j=0; j < num_nodes; j++){
            if((int(N.find(nodes[next])) < num_nodes) && (adj_map[next][j] != INT_MAX) && (get<0>(distance[next]) + adj_map[next][j] < get<0>(distance[j]))){
                get<0>(distance[j]) = get<0>(distance[next]) + adj_map[next][j];
                get<1>(distance[j]) = nodes[next];
            }
        }
        
        //copy current distances over to row i of table
        for(int j=0; j < num_nodes; j++){
            D[i][j] = distance[j];
        }
    }
    
    cout << endl;
    
    //print out shortest path table
    cout << setw(25) << "";
    for(int i=0; i < num_nodes; i++){
        string t = "";
        t += nodes[i];
        string str = "D(" + t + "),p(" + t + ")";
        cout << setw(25) << std::left << str;
    }
    cout << endl;
    
    cout << setw(25) << "";
    for(int i=0; i < num_nodes; i++){
        cout << setw(25) << std::left << "________";
    }
    cout << endl;
    
    for(int i=0; i < num_nodes; i++){
        cout << setw(25) << std::left << V[i];
        for(int j=0; j < num_nodes; j++){
            string t1 = "";
            string t2 = "";
            t1 += to_string(get<0>(D[i][j]));
            t2 += get<1>(D[i][j]);
            string str = t1 + "," + t2;
            
            cout << setw(25) << std::left << str;
        }
        cout << endl;
    }

    return 0;
}
