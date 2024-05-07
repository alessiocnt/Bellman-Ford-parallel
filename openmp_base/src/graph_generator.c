#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h> 
#include "../libs/graph.h"
#include "../libs/bellmanFord.h"
#include "../libs/utils.h"

// Generate a random weighted (connected) graph with (no) negative cycles
int** generateGraph(int V, int E, bool ensureConnectness, int minWeight, int maxWeight) {
    srand(time(NULL));
    int** graph = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        graph[i] = (int*)malloc(V * sizeof(int));
    }
    // Graph initialization with INIT
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = INIT;
        }
    }
    // Ensure the graph is connected by creating a spanning tree
    int remainingEdges = E;
    if (ensureConnectness == true) {
        for (int i = 1; i < V; i++) {
            int randomWeight = (rand() % (maxWeight - minWeight + 1)) + minWeight; // Random weight between minWeight and maxWeight
            int randomVertex = rand() % i;
            graph[i][randomVertex] = randomWeight;
        }
        remainingEdges = E - (V - 1);
    }
    // Add remaining edges randomly
    for (int i = 0; i < remainingEdges; i++) {
        int randomWeight = (rand() % (maxWeight - minWeight + 1)) + minWeight;
        int u = rand() % V;
        int v = rand() % V;
        while (u==v || graph[u][v] != INIT) {
            u = rand() % V;
            v = rand() % V;
        }
        graph[u][v] = randomWeight;
    }
    return graph;
}

int main() {
    int V = 5;
    int E = 8;
    int minWeight = -5;
    int maxWeight = 10;
    bool ensureConnectness = true;
    bool nonNegativeCycles = true;
    char path[] = "./benchmark/graph_M_0_5_750_5_8.txt";
    // Check if E is feasible
    if (E < (V-1) || E > (V*V-V)){
        printf("Number of edges is not feasible.\n");
        return 1;
    }
    int** g;
    // Ensure no negative cycles
    if (nonNegativeCycles == true) {
        int nonNeg = -1;
        while (nonNeg == -1) {
            g = generateGraph(V, E, ensureConnectness, minWeight, maxWeight); // graph as a matrix
            struct Graph* graph = createGraph(V, E); // graph as a struct
            int count = 0;
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    if(g[i][j] != INIT){
                        //printf("Edge %d: %d %d %d\n", count, i, j, g[i][j]);
                        graph->edge[count].src = i;
                        graph->edge[count].dest = j;
                        graph->edge[count].weight = g[i][j];
                        count++;
                    }
                }
            }
            nonNeg = bellmanFord(graph, 0);
            free(graph->edge);
            free(graph);
        }
    } else {
        g = generateGraph(V, E, ensureConnectness, minWeight, maxWeight);
    }
    printGraphToFile(g, V, E, path);
    for (int i = 0; i < V; i++) {
        free(g[i]);
    }
    free(g);
    return 0;
}
