#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ITERATIONS 5

double gettime( void )
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts );
    return (ts.tv_sec + (double)ts.tv_nsec / 1e9);
}

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
    graph->edge = (struct Edge*)malloc(E * sizeof(struct Edge));
    return graph;
}

char* getFilename(char *filename) {
	char *path = "./benchmark/";
	char *fullPath = (char*)malloc(strlen(path) + strlen(filename) + 1);
	strcpy(fullPath, path);
	strcat(fullPath, filename);
	return fullPath;
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
void freeTemporaryDevice(int* d_dist, int* d_isNegativeCycle) {
    cudaFree(d_isNegativeCycle); // Free isNegativeCycle
    cudaFree(d_dist); // Free distances
}

void freeGraphDevice(struct Graph* d_graph) {
    cudaFree(&(d_graph->edge)); // Free edges
    cudaFree(d_graph); // Free graph
}

// Free host memory
void freeHost(struct Graph* h_graph) {
    free(h_graph->edge); // Free edges
    free(h_graph); // Free graph
}

void freeTemporaryHost(int* h_dist) {
    free(h_dist); // Free distances
}

__global__ void bellmanFord(Graph *d_graph, int *d_dist) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < d_graph->E) {
        int u = d_graph->edge[i].src;
        int v = d_graph->edge[i].dest;
        int weight = d_graph->edge[i].weight;

        if (d_dist[u] != INT_MAX && d_dist[u] + weight < d_dist[v]) {
            atomicMin(&d_dist[v], d_dist[u] + weight);
        }
    }
}

__global__ void checkNegativeWeightCycles(Graph *d_graph, int *d_dist, int *d_isNegativeCycle) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < d_graph->E) {
        int u = d_graph->edge[i].src;
        int v = d_graph->edge[i].dest;
        int weight = d_graph->edge[i].weight;

        if (d_dist[u] != INT_MAX && d_dist[u] + weight < d_dist[v]) {
            d_dist[v] = -INT_MAX;
            *d_isNegativeCycle = 1;
        }
    }
}

int main(int argc, char* argv[]) {
    char** file_names;
	if (argc == 2) {
		file_names = (char**)malloc(2 * sizeof(const char*));
		file_names[0] = argv[1];
		file_names[1] = NULL; // Mark the end of the array
	} else {
		file_names = (char**)malloc(5 * sizeof(const char*));
        file_names[0] = "graph_XS.txt";
        file_names[1] = "graph_S.txt";
	file_names[2] = "graph_M.txt";
	file_names[3] = "graph_L.txt";
        file_names[4] = NULL;
	}
	printf("\n ---- CUDA ----\n");
    // Loop through each file name
    int file_number = 0;
	char *path;
    while (file_names[file_number] != NULL) {
		path = getFilename(file_names[file_number]);
        struct Graph *h_graph, *d_graph;
        // Host graph structures
        printf("Importing %s ...\n", file_names[file_number]);
        h_graph = importGraphFromFile(path);
        int V = h_graph->V;
        int E = h_graph->E;
        printf("Graph imported.\n");
        // Device graph structures
        cudaMalloc((void**)&d_graph, sizeof(struct Graph));
        cudaMemcpy(d_graph, h_graph, sizeof(struct Graph), cudaMemcpyHostToDevice);
        Edge *d_edge;
        cudaMalloc((void**)&d_edge, E*sizeof(struct Edge));
        cudaMemcpy(d_edge, h_graph->edge, E*sizeof(struct Edge), cudaMemcpyHostToDevice);
        cudaMemcpy(&(d_graph->edge), &d_edge, sizeof(Edge*), cudaMemcpyHostToDevice);

        double timing[ITERATIONS];
		double totalTime = 0;

        for (int i=0; i<ITERATIONS; i++) {
            // Host initialization
            int h_isNegativeCycle = 0;
            int *h_dist, *d_dist, *d_isNegativeCycle;
            h_dist = (int*)malloc(V * sizeof(int));
            // Initialize distances from src to all other vertices as infinite
            for (int j=0; j<V; j++)
                h_dist[j] = INT_MAX;
            h_dist[0] = 0;
            // Device initialization
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
            for (int j=0; j<(V-1); j++) {
                bellmanFord<<<numBlocks, blockSize>>>(d_graph, d_dist);
                cudaDeviceSynchronize(); // Ensure all threads finish before next iteration
            }
            // Run kernel to check for negative-weight cycles
            checkNegativeWeightCycles<<<numBlocks, blockSize>>>(d_graph, d_dist, d_isNegativeCycle);
            cudaDeviceSynchronize(); // Ensure all threads finish before copying result back to host
            double tend = gettime();
            double elapsed = tend - tstart; 
            // printf("Elapsed time in seconds for parallel implementation: %f\n", elapsed);
            timing[i] = elapsed;
            
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
            freeTemporaryDevice(d_dist, d_isNegativeCycle);
            freeTemporaryHost(h_dist);
        }
        printf("Time for execution: ");
        for (int j = 0; j<ITERATIONS; j++) {
            printf("%f ", timing[j]);
            totalTime += timing[j];
        }
        printf("\n Average time: %f\n", totalTime/ITERATIONS);
        printf("\n ---------------------- \n");
        totalTime = 0;
        file_number++;
        freeHost(h_graph);
        freeGraphDevice(d_graph);   
    }

    return EXIT_SUCCESS;
}
