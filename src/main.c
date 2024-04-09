#include "bellmanFord.h"
#include "graph.h"
#include "utils.h"

int main()
{
	struct Graph* graph = importGraphFromFile("../benchmark/graph_M_3.txt");
	bellmanFord(graph, 0);
	return 0;
}