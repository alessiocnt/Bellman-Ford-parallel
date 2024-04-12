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

void initializeNode(struct Graph* graph, int node, int outSize) {
    graph->nodes[node].value = node;
    graph->nodes[node].iterationCount = 0;
    graph->nodes[node].outEdges = (struct Edge*)malloc(outSize * sizeof(struct Edge));
}

struct Node nullNode = {-1, -1, NULL, -1};
