#include "bellmanFord.h"
#include "graph.h"
#include "utils.h"
#include <omp.h>

int main()
{
	struct Graph* graph = importGraphFromFile("./benchmark/graph_003_M.txt");
	omp_set_num_threads(4);
	bellmanFord_parallel(graph, 0);
	return 0;
}