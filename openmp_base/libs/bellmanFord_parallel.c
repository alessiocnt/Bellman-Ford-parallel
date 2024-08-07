#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>
#include "bellmanFord.h"
#include "graph.h"
#include "utils.h"

int bellmanFord_parallel(struct Graph* graph, int src) {
    int V = graph->V;
    int E = graph->E;
    int* dist = (int*)malloc(V * sizeof(int));
    // Initialize distances from src to all other vertices as infinite
    #pragma omp parallel for 
    for (int i=0; i<V; i++)
        dist[i] = INT_MAX;
    // Implicit barrier
    dist[src] = 0;

    // Relax all edges |V|-1 times
    for (int i=1; i<=V-1; i++) {
        #pragma omp parallel for
        for (int j=0; j<E; j++) {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int weight = graph->edge[j].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                #pragma omp critical
                dist[v] = dist[u] + weight;
        }
    }
    // Check for negative-weight cycles
    int result = 0;
    #pragma omp parallel for
    for (int i=0; i<E; i++) {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int weight = graph->edge[i].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            #pragma omp critical
            {
                result = -1;
            }
        }
    }
    if (result == -1){
        printf("Graph contains negative weight cycle\n");
        free(dist);
        return -1;
    } 
    else {
        // Print distances
        // printf("Vertex   Distance from Source\n");
        // for (int i = 0; i < V; i++)
        //     printf("%d \t\t %d\n", i, dist[i]);
        free(dist);
        return 0;
    }
}
