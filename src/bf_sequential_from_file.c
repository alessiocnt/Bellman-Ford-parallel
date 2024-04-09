#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// A structure to represent a weighted edge in graph
struct Edge {
	int src, dest, weight;
};
// A structure to represent a connected, directed and weighted graph
struct Graph {
    int V, E;
    struct Edge* edge;
};
// Creates a graph
// V: number of vertices, E: number of edges
struct Graph* createGraph(int V, int E) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (struct Edge*)malloc(graph->E * sizeof(struct Edge));
    return graph;
}

void bellmanFord(struct Graph* graph, int src) {
    int V = graph->V;
    int E = graph->E;
    int* dist = (int*)malloc(V * sizeof(int));
    // Initialize distances from src to all other vertices as infinite
    for (int i=0; i<V; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;
    // Relax all edges |V|-1 times
    for (int i=1; i<=V-1; i++) {
        for (int j=0; j<E; j++) {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int weight = graph->edge[j].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
        }
    }
    // Check for negative-weight cycles
    for (int i=0; i<E; i++) {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int weight = graph->edge[i].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }
    }
    // Print distances
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t %d\n", i, dist[i]);
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
				if(tmp != INT_MAX){
					printf("Edge %d: %d %d %d\n", count, i, j, tmp);
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

int main()
{
	struct Graph* graph = importGraphFromFile("../benchmark/graph_M_3.txt");
	bellmanFord(graph, 0);
	return 0;
}
