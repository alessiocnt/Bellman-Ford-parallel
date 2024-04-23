#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "graph.h"

double gettime( void )
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts );
    return (ts.tv_sec + (double)ts.tv_nsec / 1e9);
}

// Import graph from a file
struct Graph *importGraphFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
	int V, E;
    if (file != NULL) {
		fscanf(file, "%d", &V);
		fscanf(file, "%d", &E);
		struct Graph* graph = createGraph(V, E);
		int count = 0;
		int tmp;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                fscanf(file, "%d", &tmp);
				if(tmp != 0){
					// printf("Edge %d: %d %d %d\n", count, i, j, tmp);
					graph->edge[count].src = i;
					graph->edge[count].dest = j;
					graph->edge[count].weight = tmp;
					count++;
				}
			}
    	}
		fclose(file);
		return graph;
	} else {
		printf("Error opening file for reading.\n");
		return NULL;
    }
    return NULL;
}

// Free device memory
void freeDevice(struct Graph* d_graph, int* d_dist, int* d_isNegativeCycle) {
    cudaFree(d_isNegativeCycle); // Free isNegativeCycle
    cudaFree(d_dist); // Free distances
    cudaFree(&(d_graph->edge)); // Free edges
    cudaFree(d_graph); // Free graph
}

// Free host memory
void freeHost(struct Graph* h_graph, int* h_dist) {
    free(h_graph->edge); // Free edges
    free(h_graph); // Free graph
    free(h_dist); // Free distances
}