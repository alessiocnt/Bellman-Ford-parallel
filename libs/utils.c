#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "graph.h"

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
                if(tmp != INIT){
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
}

// TODO: Check
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