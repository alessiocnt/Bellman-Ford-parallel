#include <omp.h>
#include "graph.h"
#include "bellmanFord.h"
#include "utils.h"

int main()
{
	struct Graph* graph = importGraphFromFile("./benchmark/graph_001_XS.txt");
	omp_set_num_threads(4);
	bellmanFord_parallel_hbf(graph, 0);
	return 0;
}