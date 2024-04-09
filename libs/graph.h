#ifndef GRAPH_H
	#define GRAPH_H
    // Structure to represent a weighted edge in graph
    struct Edge {
        int src, dest, weight;
    };
    // Structure to represent a connected, directed and weighted graph
    struct Graph {
        int V, E;
        struct Edge* edge;
    };
    // Creates a graph with V vertices and E edges
    struct Graph* createGraph(int V, int E);
#endif
