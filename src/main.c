/****************************************************************************
*
* Run with:
* ./bellman_ford filename n_threads
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "graph.h"
#include "bellmanFord.h"
#include "utils.h"

#define ITERATIONS 5

struct Graph* loadGraph(char *filename) {
	printf("Loading graph %s ..\n", filename);
	double istart = omp_get_wtime(); 
	struct Graph* graph = importGraphFromFile(filename);
	double ielapsed = omp_get_wtime() - istart;
	printf("Time for graph load %f\n", ielapsed);
	return graph;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
        printf("Specify the <filename> of the graph you want to process.\n");
        return EXIT_FAILURE;
    }
    // char *filename = strcat("./benchmark/", argv[1]);
	char *filename = "./benchmark/graph_001_XS.txt";
	// Compute threads availability and the series to test 
	int availableThreads = omp_get_max_threads();
    int* threadSeries;
    int threadSeriesLen = computeThreadSeries(availableThreads, &threadSeries);
	printf("Available threads: %d\n", availableThreads);

	struct Graph* graph = loadGraph(filename);

	// Run the algorithm with the series of threads
	double timing[ITERATIONS];
	double totalTime = 0;
    for (int i = 0; i<threadSeriesLen; i++) {
        for (int j = 0; j<ITERATIONS; j++) {
			double tstart = omp_get_wtime(); 
			bellmanFord_parallel_hbf(graph, INIT);
			double elapsed = omp_get_wtime() - tstart;  
			timing[j] = elapsed;
		}
		printf("Bellman-Ford executed with %d threads \n", threadSeries[i]);
		printf("Time for execution: ");
		for (int j = 0; j<ITERATIONS; j++) {
			printf("%f ", timing[j]);
			totalTime += timing[j];
		}
		printf("\n Average time: %f\n", totalTime/ITERATIONS);
		printf("\n ---------------------- \n");
		totalTime = 0;
    }
    free(threadSeries);
	freeGraph(graph);
    return EXIT_SUCCESS;
}



// int main(int argc, char* argv[])
// {
// 	if (argc == 2) {
// 		int threadNum = atoi(argv[1]);
// 		omp_set_num_threads(threadNum);
//     } 
// 	double istart = omp_get_wtime(); 
// 	struct Graph* graph = importGraphFromFile("./benchmark/graph_005_XL.txt");
// 	double ielapsed = omp_get_wtime() - istart;  
	
// 	double tstart = omp_get_wtime(); 
// 	bellmanFord_parallel_hbf(graph, 0);
// 	double elapsed = omp_get_wtime() - tstart;  

// 	printf("Executed concurrently with %d threads \n", omp_get_max_threads());
//     printf("Elapsed time %f\n", elapsed);
// 	printf("Time for graph load %f\n", ielapsed);
//     return EXIT_SUCCESS;
// 	// return 0;
// }