#include <stdio.h>
#include <stdlib.h>
#include "bellmanFord.h"
#include "graph.h"
#include "utils.h"

int main()
{
    printf("MAIN\n");
	struct Graph *h_graph, *d_graph;
    int h_isNegativeCycle = 0;
    int *h_dist, *d_dist, *d_isNegativeCycle;
    // Host structures
    printf("Create graph...\n");
    h_graph = importGraphFromFile("./graph_test.txt");
    int V = h_graph->V;
    int E = h_graph->E;
    printf("Graph imported\n");

    h_dist = (int*)malloc(V * sizeof(int));
    // Initialize distances from src to all other vertices as infinite
    for (int i=0; i<V; i++)
        h_dist[i] = INT_MAX;
    h_dist[0] = 0;

    // Device structures
    // Graph structure
    cudaMalloc((void**)&d_graph, sizeof(struct Graph));
    cudaMemcpy(d_graph, h_graph, sizeof(struct Graph), cudaMemcpyHostToDevice);
    Edge *d_edge;
    cudaMalloc((void**)&d_edge, E*sizeof(struct Edge));
    cudaMemcpy(d_edge, h_graph->edge, E*sizeof(struct Edge), cudaMemcpyHostToDevice);
    cudaMemcpy(&(d_graph->edge), &d_edge, sizeof(Edge*), cudaMemcpyHostToDevice);
    // Distances
    cudaMalloc((void **)&d_dist, V*sizeof(int));
    cudaMemcpy(d_dist, h_dist, V*sizeof(int), cudaMemcpyHostToDevice);
    // Is negative cycle
    cudaMalloc((void**)&d_isNegativeCycle, sizeof(int));
    cudaMemcpy(d_isNegativeCycle, &h_isNegativeCycle, sizeof(int), cudaMemcpyHostToDevice);
	
    // Define grid and block dimensions
    int blockSize = 256;
    int numBlocks = (E + blockSize - 1) / blockSize;

    double tstart = gettime();
    // Run kernel V-1 times
    for (int i=0; i<(V-1); i++) {
        bellmanFord<<<numBlocks, blockSize>>>(d_graph, d_dist);
        cudaDeviceSynchronize(); // Ensure all threads finish before next iteration
    }
    // Run kernel to check for negative-weight cycles
    checkNegativeWeightCycles<<<numBlocks, blockSize>>>(d_graph, d_dist, d_isNegativeCycle);
    cudaDeviceSynchronize(); // Ensure all threads finish before copying result back to host
    double tend = gettime();
    printf("Elapsed time in seconds for parallel implementation: %f\n", tend-tstart);
    
    // Copy result back to host
    cudaMemcpy(&h_isNegativeCycle, d_isNegativeCycle, sizeof(int), cudaMemcpyDeviceToHost);
    if (h_isNegativeCycle) {
        printf("Graph contains negative weight cycle\n");
        return 0;
    } 
    cudaMemcpy(h_dist, d_dist, V * sizeof(int), cudaMemcpyDeviceToHost);
    // Print distances
    // printf("Vertex   Distance from Source\n");
    // for (int i = 0; i < V; i++) {
    //     printf("%d \t\t %d\n", i, h_dist[i]);
    // }

    freeHost(h_graph, h_dist);
    freeDevice(d_graph, d_dist, d_isNegativeCycle);
    return 0;
}