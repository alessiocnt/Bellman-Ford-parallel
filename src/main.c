/****************************************************************************
*
* Run with:
* ./bellman_ford n_threads
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "graph.h"
#include "bellmanFord.h"
#include "utils.h"

int main(int argc, char* argv[])
{
	float tstart, elapsed;
	if (argc == 2) {
		int threadNum = atoi(argv[1]);
		omp_set_num_threads(threadNum);
    } 
	struct Graph* graph = importGraphFromFile("./benchmark/graph_003_M.txt");
	
	tstart = omp_get_wtime();
	bellmanFord_parallel_hbf(graph, 0);
	elapsed = omp_get_wtime() - tstart;  

	printf("Executed concurrently with %d threads \n", omp_get_max_threads());
    printf("Elapsed time %f\n", elapsed);
    return EXIT_SUCCESS;
	// return 0;
}