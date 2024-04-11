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
	if (argc == 2) {
		int threadNum = atoi(argv[1]);
		omp_set_num_threads(threadNum);
    } 
	double istart = omp_get_wtime(); 
	struct Graph* graph = importGraphFromFile("./benchmark/graph_005_XL.txt");
	double ielapsed = omp_get_wtime() - istart;  
	
	double tstart = omp_get_wtime(); 
	bellmanFord_parallel_hbf(graph, 0);
	double elapsed = omp_get_wtime() - tstart;  

	printf("Executed concurrently with %d threads \n", omp_get_max_threads());
    printf("Elapsed time %f\n", elapsed);
	printf("Time for graph load %f\n", ielapsed);
    return EXIT_SUCCESS;
	// return 0;
}