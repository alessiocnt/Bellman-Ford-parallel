#ifndef FRONTIER_H
	#define FRONTIER_H
    #include "graph.h"
    
    // Structure to represent a frontiere of active nodes in the graph
    struct Frontier {
        int front, rear;
        int capacity;
        struct Node* nodes;
    };
    struct Frontier* createFrontier(int capacity);
    int isEmpty(struct Frontier* frontier);
    int isFull(struct Frontier* frontier);
    int getLength(struct Frontier* f);
    void enqueue(struct Frontier* frontier, struct Node node);
    struct Node dequeue(struct Frontier* frontier);
    void* swap(struct Frontier **f1, struct Frontier **f2);
    
#endif
