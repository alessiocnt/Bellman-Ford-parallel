#ifndef BELLMANFORD_H
	#define BELLMANFORD_H
	#include "graph.h"
    // Bellman Ford algorithm, returns 0 if a solution is given, -1 if there is a negative cycle
<<<<<<<< HEAD:openmp_frontier/libs/bellmanFord.h
	int bellmanFord_parallel_hbf(struct Graph* graph, int src);
========
	int bellmanFord(struct Graph* graph, int src);
	int bellmanFord_parallel(struct Graph* graph, int src);
>>>>>>>> develop:openmp_base/libs/bellmanFord.h
#endif