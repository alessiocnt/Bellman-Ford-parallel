#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "graph.h"

void countEdges(int V, FILE *file, int* outAdjLen) {
    int w;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            fscanf(file, "%d", &w);
            if(w != INIT){
                outAdjLen[i]++;
            }
        }
    }
}

// Import graph from a file
struct Graph *importGraphFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    int V, E;
    if (file != NULL) {
        fscanf(file, "%d", &V);
        fscanf(file, "%d", &E);
        // Create and size the graph
        struct Graph* graph = createGraph(V, E); // Create graph with V vertices and E edges
        // Count outcoming edges for each vertex
        int* outAdjLen = (int*)calloc(V, sizeof(int));
        countEdges(V, file, outAdjLen); 
        // Properly size the incoming and outgoing edges lists for each node in the graph
        for (int i = 0; i < V; i++) {
            initializeNode(graph, i, outAdjLen[i]);
        }
        // Populate the graph with nodes and edges
        fseek(file, 0, SEEK_SET);
        fscanf(file, "%d", &V); // Skip
        fscanf(file, "%d", &E); // Skip
        int outCounter = 0;
        int w;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                fscanf(file, "%d", &w);
                if(w != INIT){
                    // Populate list of edges outgoing from i to j
                    graph->nodes[i].outEdges[outCounter].weight = w;
                    graph->nodes[i].outEdges[outCounter].src = &graph->nodes[i];
                    graph->nodes[i].outEdges[outCounter].dest = &graph->nodes[j];
                    outCounter++;
                }
            }
            graph->nodes[i].outEdgesSize = outCounter;
            outCounter = 0;
        }
        fclose(file);
        return graph;
    } else {
        printf("Error opening file for reading.\n");
        return NULL;
    }
}

// Computes a series of threads to be used in parallel computation
// Returns the size of the series and modifies the pointer to the series
int computeThreadSeries(int N, int** series) {
    int size = 1;
    int val = 1;
    while (val <= N){
        val *= 2;
        size++;
    }
    if (N%2 != 0){
        size++;
    }
    *series = (int*)malloc(size * sizeof(int));
    if (*series == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int value = 1;
    for (int i = 0; i < size; i++) {
        (*series)[i] = value;
        value *= 2;
    }
    return size;
}

// TODO
// Export graph to a file
void printGraphToFile(int **matrix, int V, int E, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%d\n", V);
        fprintf(file, "%d\n", E);
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                fprintf(file, "%d ", matrix[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    } else {
        printf("Error opening file for writing.\n");
    }
}