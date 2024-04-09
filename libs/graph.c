#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

// Creates a graph with V vertices and E edges
struct Graph* createGraph(int V, int E) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->E = E;
    graph->nodes = (struct Node*)malloc(graph->V * sizeof(struct Node));
    return graph;
}

void* createEdges(struct Graph* graph, int node, int inSize, int outSize) {
    graph->nodes[node].value = node;
    graph->nodes[node].inEdges = (struct Edge*)malloc(inSize * sizeof(struct Edge));
    graph->nodes[node].outEdges = (struct Edge*)malloc(outSize * sizeof(struct Edge));
}
