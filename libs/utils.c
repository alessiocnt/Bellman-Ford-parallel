#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "graph.h"

struct CountEdge {
    int in = 0;
    int out = 0;
};

struct CountEdge* countEdges(int V) {
    CountEdge* counter = (CountEdge*)malloc(V * sizeof(CountEdge));
    int w;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            fscanf(file, "%d", &w);
            if(w != INIT){
                counter[i].out++;
                counter[j].in++;
            }
        }
    }
    return counter;
}

// Import graph from a file
struct Graph *importGraphFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    int V, E;
    if (file != NULL) {
        fscanf(file, "%d", &V);
        fscanf(file, "%d", &E);
        struct Graph* graph = createGraph(V, E); // Create graph with V vertices and E edges
        struct CountEdge* counter = countEdges(V); // Count in and out edges for each vertex
        // Properly size the inedge and outedge for each node of the graph
        for (int i = 0; i < V; i++) {
            createEdges(graph, i, counter[i].in, counter[i].out);
        }
        fseek(file, 0, SEEK_SET); //Reset file pointer to the beginning of the file
        fscanf(file, "%d", &V); // Skip the first line
        fscanf(file, "%d", &E); // Skip the second line
        int outCounter = 0;
        int inCounter[V];
        int w;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                fscanf(file, "%d", &w);
                if(w != INIT){
                    // Populate list of edges outgoing from i to j
                    graph->nodes[i].outEdges[outCounter].weight = w;
                    graph->nodes[i].outEdges[outCounter].src = graph->nodes[i];
                    graph->nodes[i].outEdges[outCounter].dest = graph->nodes[j];
                    // Populate the list of edges incoming to j from i
                    graph->nodes[j].inEdges[inCounter[j]].weight = w;
                    graph->nodes[j].inEdges[inCounter[j]].src = graph->nodes[i];
                    graph->nodes[j].inEdges[inCounter[j]].dest = graph->nodes[j];
                    outCounter++;
                    inCounter[j]++;
                }
            }
            outCounter = 0;
        }
        fclose(file);
        return graph;
    } else {
        printf("Error opening file for reading.\n");
        return NULL;
    }
}

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