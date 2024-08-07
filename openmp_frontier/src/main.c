/****************************************************************************
*
* Run with:
* ./bellman_ford filename
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

char* getFilename(char *filename) {
	char *path = "./benchmark/";
	char *fullPath = malloc(strlen(path) + strlen(filename) + 1);
	strcpy(fullPath, path);
	strcat(fullPath, filename);
	return fullPath;
}

int main(int argc, char* argv[]) {
	char** file_names;
	if (argc == 2) {
		file_names = (char**)malloc(2 * sizeof(const char*));
		file_names[0] = argv[1];
		file_names[1] = NULL; // Mark the end of the array
	} else {
		file_names = (char**)malloc(5 * sizeof(const char*)); 
	file_names[0] = "graph_XS.txt"; 
        file_names[1] = "graph_S.txt"; 
	file_names[2] = "graph_M.txt"; 
	file_names[3] = "graph_L.txt"; 
        file_names[4] = NULL;
	}
	printf("\n ---- OpenMP Frontier ----\n");
    // Compute threads availability and the series to test 
	int availableThreads = omp_get_max_threads();
    int* threadSeries;
    int threadSeriesLen = computeThreadSeries(availableThreads, &threadSeries);
	printf("Available physical threads: %d\n", availableThreads);

	// Loop through each file name
    int i = 0;
	char *path;
    while (file_names[i] != NULL) {
		path = getFilename(file_names[i]);
		struct Graph* graph = loadGraph(path);
		// Run the algorithm with the series of threads
		double timing[ITERATIONS];
		double totalTime = 0;
		for (int i = 0; i<threadSeriesLen; i++) {
			omp_set_num_threads(threadSeries[i]);
			for (int j = 0; j<ITERATIONS; j++) {
				double tstart = omp_get_wtime(); 
				bellmanFord_parallel_hbf(graph, INIT);
				double elapsed = omp_get_wtime() - tstart;  
				timing[j] = elapsed;
			}
			printf("Bellman-Ford executed with %d threads \n", omp_get_max_threads());
			printf("Time for execution: ");
			for (int j = 0; j<ITERATIONS; j++) {
				printf("%f ", timing[j]);
				totalTime += timing[j];
			}
			printf("\n Average time: %f\n", totalTime/ITERATIONS);
			printf("\n ---------------------- \n");
			totalTime = 0;
		}
		freeGraph(graph);
		i++;
	}
	
    free(threadSeries);
    return EXIT_SUCCESS;
}
