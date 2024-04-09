#ifndef GRAPH_H
	#define GRAPH_H
    // Structure to represent a node in graph
    struct Node {
        int value;
        struct Edge* outEdges;
        struct Edge* inEdges;
    };    

    // Structure to represent a weighted edge in graph
    struct Edge {
        int weight;
        struct Node* src;
        struct Node* dest;
    };
    // Structure to represent a connected, directed and weighted graph
    struct Graph {
        int V, E;
        struct Node* nodes;
    };
    // Creates a graph with V vertices and E edges
    struct Graph* createGraph(int V, int E);
#endif
