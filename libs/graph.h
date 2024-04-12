#ifndef GRAPH_H
	#define GRAPH_H
    #include <stddef.h>
    // Structure to represent a node in graph
    struct Node {
        int value;
        int iterationCount;
        // Lists of edges outgoing and incoming
        struct Edge* outEdges;
        int outEdgesSize;
    };

    extern struct Node nullNode;

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
    // Initializes a node in the graph
    void initializeNode(struct Graph* graph, int node, int outSize);
#endif
