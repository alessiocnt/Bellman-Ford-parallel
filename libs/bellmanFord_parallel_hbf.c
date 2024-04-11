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
    for (int i=0; i<V; i++) {
        dist[i] = INT_MAX;
    }
    dist[src] = 0;

    struct Frontier* f1 = createFrontier(V); // Set capacity to the max number of vertex
    enqueue(f1, struct Edge node);// TODO create src node
    struct Frontier* f2 = createFrontier(V); // Empty

    // currentIteration = 0 -> see paper
    currentIteration = 0;
    while (!isEmpty(f1) && currentIteration<V+1) { // Early exit if frontier gets empty, otherwise |V|+1 iterations let us detect negative cycles
        #pragma omp parallel for
        for(int i=0; i<getLength(f1); i++) {
            struct Node* source = dequeue(f1);
            #pragma omp parallel for
            for (int j=0; j<source.outEdgesSize; j++) {
                struct Node* dest = source.outEdges[j].dest;
                if (dest.itererationCount != currentIteration){ // Insert node only once in the frontier
                    // Relax operation
                    int u = source.value;
                    int v = dest.value;
                    int weight = source.outEdges[j].weight;
                    if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                        #pragma omp critical
                        dist[v] = dist[u] + weight;
                        enqueue(f2, dest);
                        dest.iterationCount = currentIteration;
                    }
                }
            }
        }
        swap(&f1, &f2);
        currentIteration++;
    }
    // Print distances or detect negative weight cycle
    if (currentIteration == V+1) {
        printf("Graph contains negative weight cycle\n");
        return -1;
    } else {
        // Print distances
        printf("Vertex   Distance from Source\n");
        for (int i = 0; i < V; i++)
            printf("%d \t\t %d\n", i, dist[i]);
        return 0;
    }
}
