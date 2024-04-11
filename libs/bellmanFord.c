// #include <stdio.h>
// #include <stdlib.h>
// #include <limits.h>
// #include "bellmanFord.h"
// #include "graph.h"
// #include "utils.h"

// int bellmanFord(struct Graph* graph, int src) {
//     int V = graph->V;
//     int E = graph->E;
//     int* dist = (int*)malloc(V * sizeof(int));
//     // Initialize distances from src to all other vertices as infinite
//     for (int i=0; i<V; i++)
//         dist[i] = INT_MAX;
//     dist[src] = 0;
//     // Relax all edges |V|-1 times
//     for (int i=1; i<=V-1; i++) {
//         for (int j=0; j<E; j++) {
//             int u = graph->edge[j].src;
//             int v = graph->edge[j].dest;
//             int weight = graph->edge[j].weight;
//             if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
//                 dist[v] = dist[u] + weight;
//         }
//     }
//     // Check for negative-weight cycles
//     for (int i=0; i<E; i++) {
//         int u = graph->edge[i].src;
//         int v = graph->edge[i].dest;
//         int weight = graph->edge[i].weight;
//         if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
//             printf("Graph contains negative weight cycle\n");
//             return -1;
//         }
//     }
//     // Print distances
//     printf("Vertex   Distance from Source\n");
//     for (int i = 0; i < V; i++)
//         printf("%d \t\t %d\n", i, dist[i]);
//     return 0;
// }
