#include <stdio.h>
#include <stdlib.h>
#include "bellmanFord.h"
#include "graph.h"

__global__ void bellmanFord(Graph *d_graph, int *d_dist) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < d_graph->E) {
        int u = d_graph->edge[i].src;
        int v = d_graph->edge[i].dest;
        int weight = d_graph->edge[i].weight;

        if (d_dist[u] != INT_MAX && d_dist[u] + weight < d_dist[v]) {
            atomicMin(&d_dist[v], d_dist[u] + weight);
        }
    }
}

__global__ void checkNegativeWeightCycles(Graph *d_graph, int *d_dist, int *d_isNegativeCycle) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < d_graph->E) {
        int u = d_graph->edge[i].src;
        int v = d_graph->edge[i].dest;
        int weight = d_graph->edge[i].weight;

        if (d_dist[u] != INT_MAX && d_dist[u] + weight < d_dist[v]) {
            d_dist[v] = -INT_MAX;
            *d_isNegativeCycle = 1;
        }
    }
}