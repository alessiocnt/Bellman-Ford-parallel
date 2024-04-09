#ifndef FRONTIER_H
	#define FRONTIER_H
    #include "graph.h"
    
    // Structure to represent a frontiere of active nodes in the graph
    struct Frontier {
        int front, rear;
        int capacity;
        struct Edge* nodes;
    };
    struct Frontier* createFrontier(int capacity);
    int isEmpty(struct Frontier* frontier);
    int isFull(struct Frontier* frontier);
    void enqueue(struct Frontier* frontier, struct Edge node);
    struct Edge dequeue(struct Frontier* frontier);
    
#endif
