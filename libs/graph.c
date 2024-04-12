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
// Initializes a node in the graph
void initializeNode(struct Graph* graph, int node, int outSize) {
    graph->nodes[node].value = node;
    graph->nodes[node].iterationCount = 0;
    graph->nodes[node].outEdges = (struct Edge*)malloc(outSize * sizeof(struct Edge));
}
// Frees the memory allocated for the graph
void freeGraph(struct Graph* graph) {
    int V = graph->V;
    for (int i = 0; i < V; i++) {
        free(graph->nodes[i].outEdges);
    }
    free(graph->nodes);
    free(graph);
}

struct Node nullNode = {-1, -1, NULL, -1};
