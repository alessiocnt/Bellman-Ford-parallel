#ifndef BELLMANFORD_H
	#define BELLMANFORD_H
	#include "graph.h"
    // Bellman Ford algorithm, returns 0 if a solution is given, -1 if there is a negative cycle
	int bellmanFord_parallel_hbf(struct Graph* graph, int src);
#endif