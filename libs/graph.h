#ifndef GRAPH_H
	#define GRAPH_H
    // Structure to represent a node in graph
    struct Node {
        int value;
        int iterationCount;
        // Lists of edges outgoing and incoming
        struct Edge* outEdges;
        int outEdgesSize;
        struct Edge* inEdges;
    };    

    // Structure to represent a weighted edge in graph
    struct Edge {
        int weight;
        // Pointers to node
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
