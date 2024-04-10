#include <stdio.h>
#include <stdlib.h>
#include "frontier.h"
#include "graph.h"
#include "graph.c"

struct Frontier* createFrontier(int capacity) {
    struct Frontier* frontier = (struct Frontier*)malloc(sizeof(struct Frontier));
    frontier->capacity = capacity;
    frontier->front = -1;
    frontier->rear = -1;
    frontier->nodes = (struct Node*)malloc(capacity * sizeof(struct Node));
    return frontier;
}

int isEmpty(struct Frontier* f) {
    return f->front == -1;
}

int isFull(struct Frontier* f) {
    return (f->rear + 1) % f->capacity == f->front;
}

void enqueue(struct Frontier* f, struct Node node) {
    #pragma omp critical 
    {
        if (isFull(f)) {
            printf("Frontier is full.\n");
        } else {
            if (isEmpty(f)) {
                f->front = 0;
                f->rear = 0;
            } else {
                f->rear = (f->rear + 1) % f->capacity;
            }
            f->nodes[f->rear] = node;
        }
    }
}

struct Node dequeue(struct Frontier* f) {
    struct Node node;
    #pragma omp critical 
    {
        if (isEmpty(f)) {
            struct Node nullNode = {-1, NULL, NULL};
            node = nullNode;
        } else {
            node = f->nodes[f->front];
            if (f->front == f->rear) {
                f->front = -1;
                f->rear = -1;
            } else {
                f->front = (f->front + 1) % f->capacity;
            }
        }
    }
    return node;
}

int main() {
    struct Frontier* f = createFrontier(5); // Set initial capacity to 5

    int V = 3; // Number of vertices in graph
	int E = 3; // Number of edges in graph
	struct Graph* graph = createGraph(V, E);

	// add node 0
    graph->nodes[0].value = 0;
    graph->nodes[0].inEdges = (struct Edge*)malloc(0 * sizeof(struct Edge));
    graph->nodes[0].outEdges = (struct Edge*)malloc(2 * sizeof(struct Edge));
    // add node 1
    graph->nodes[1].value = 1;
    graph->nodes[1].inEdges = (struct Edge*)malloc(1 * sizeof(struct Edge));
    graph->nodes[1].outEdges = (struct Edge*)malloc(1 * sizeof(struct Edge));
    // add node 2
    graph->nodes[2].value = 2;
    graph->nodes[2].inEdges = (struct Edge*)malloc(2 * sizeof(struct Edge));
    graph->nodes[2].outEdges = (struct Edge*)malloc(0 * sizeof(struct Edge));
    // populate edges
    // add edge 0-1
    graph->nodes[0].outEdges[0].weight = -1;
    graph->nodes[0].outEdges[0].src = &graph->nodes[0];
    graph->nodes[0].outEdges[0].dest = &graph->nodes[1];
    graph->nodes[1].inEdges[0].weight = -1;
    graph->nodes[1].inEdges[0].src = &graph->nodes[0];
    graph->nodes[1].inEdges[0].dest = &graph->nodes[1];
    // add edge 0-2
    graph->nodes[0].outEdges[1].weight = 4;
    graph->nodes[0].outEdges[1].src = &graph->nodes[0];
    graph->nodes[0].outEdges[1].dest = &graph->nodes[2];
    graph->nodes[2].inEdges[0].weight = 4;
    graph->nodes[2].inEdges[0].src = &graph->nodes[0];
    graph->nodes[2].inEdges[0].dest = &graph->nodes[2];
    // add edge 1-2
    graph->nodes[1].outEdges[0].weight = 2;
    graph->nodes[1].outEdges[0].src = &graph->nodes[1];
    graph->nodes[1].outEdges[0].dest = &graph->nodes[2];
    graph->nodes[2].inEdges[1].weight = 2;
    graph->nodes[2].inEdges[1].src = &graph->nodes[1];
    graph->nodes[2].inEdges[1].dest = &graph->nodes[2];	

    printf("Dequeued element: %d\n", dequeue(f).value); // Output: 1
    enqueue(f, graph->nodes[0]);
    enqueue(f, graph->nodes[1]);
    enqueue(f, graph->nodes[2]);

    printf("Dequeued element: %d\n", dequeue(f).value); // Output: 1
    printf("Dequeued element: %d\n", dequeue(f).value); // Output: 2
    printf("Dequeued element: %d\n", dequeue(f).value); // Output: 3

    return 0;
}
