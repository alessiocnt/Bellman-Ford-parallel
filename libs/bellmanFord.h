#ifndef BELLMANFORD_H
	#define BELLMANFORD_H
	#include "graph.h"
    __global__ void bellmanFord(Graph *d_graph, int *d_dist);
	__global__ void checkNegativeWeightCycles(Graph *d_graph, int *d_dist, int *d_isNegativeCycle);
#endif