#include <stdio.h>
#include <stdlib.h>
#include "frontier.h"
#include "graph.h"

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

int getLength(struct Frontier* f) {
    if (isEmpty(f)) {
        return 0;
    } else {
        return (f->capacity - f->front + f->rear + 1) % f->capacity;
    }
}

void enqueue(struct Frontier* f, struct Node* node) {
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
            f->nodes[f->rear] = *node;
        }
    }
}

struct Node* dequeue(struct Frontier* f) {
    struct Node* ptrNode = (struct Node*)malloc(sizeof(struct Node));
    #pragma omp critical 
    {
        if (isEmpty(f)) {
            //ptrNode->value = -1;
            //ptrNode->attr1 = -1;
            //ptrNode->outEdges = NULL;
            //ptrNode->attr2 = -1;
            //ptrNode->inEdges = NULL;
            // struct Node nullNode = {-1, -1, NULL, -1, NULL};
            ptrNode = &nullNode;
        } else {
            struct Node node = f->nodes[f->front];
            //ptrNode = &node;
            *ptrNode = node;
            if (f->front == f->rear) {
                f->front = -1;
                f->rear = -1;
            } else {
                f->front = (f->front + 1) % f->capacity;
            }
        }
    }
    return ptrNode;
}

void swap(struct Frontier **f1, struct Frontier **f2) {
    #pragma omp critical 
    {
        struct Frontier* temp = *f1;
        *f1 = *f2;
        *f2 = temp;
    }
}

// int main() {
//     struct Frontier* f = createFrontier(5); // Set initial capacity to 5
//     int V = 3; // Number of vertices in graph
// 	int E = 3; // Number of edges in graph
// 	struct Graph* graph = createGraph(V, E);
// 	// add node 0
//     graph->nodes[0].value = 0;
//     graph->nodes[0].inEdges = (struct Edge*)malloc(0 * sizeof(struct Edge));
//     graph->nodes[0].outEdges = (struct Edge*)malloc(2 * sizeof(struct Edge));
//     // add node 1
//     graph->nodes[1].value = 1;
//     graph->nodes[1].inEdges = (struct Edge*)malloc(1 * sizeof(struct Edge));
//     graph->nodes[1].outEdges = (struct Edge*)malloc(1 * sizeof(struct Edge));
//     // add node 2
//     graph->nodes[2].value = 2;
//     graph->nodes[2].inEdges = (struct Edge*)malloc(2 * sizeof(struct Edge));
//     graph->nodes[2].outEdges = (struct Edge*)malloc(0 * sizeof(struct Edge));
//     // populate edges
//     // add edge 0-1
//     graph->nodes[0].outEdges[0].weight = -1;
//     graph->nodes[0].outEdges[0].src = &graph->nodes[0];
//     graph->nodes[0].outEdges[0].dest = &graph->nodes[1];
//     graph->nodes[1].inEdges[0].weight = -1;
//     graph->nodes[1].inEdges[0].src = &graph->nodes[0];
//     graph->nodes[1].inEdges[0].dest = &graph->nodes[1];
//     // add edge 0-2
//     graph->nodes[0].outEdges[1].weight = 4;
//     graph->nodes[0].outEdges[1].src = &graph->nodes[0];
//     graph->nodes[0].outEdges[1].dest = &graph->nodes[2];
//     graph->nodes[2].inEdges[0].weight = 4;
//     graph->nodes[2].inEdges[0].src = &graph->nodes[0];
//     graph->nodes[2].inEdges[0].dest = &graph->nodes[2];
//     // add edge 1-2
//     graph->nodes[1].outEdges[0].weight = 2;
//     graph->nodes[1].outEdges[0].src = &graph->nodes[1];
//     graph->nodes[1].outEdges[0].dest = &graph->nodes[2];
//     graph->nodes[2].inEdges[1].weight = 2;
//     graph->nodes[2].inEdges[1].src = &graph->nodes[1];
//     graph->nodes[2].inEdges[1].dest = &graph->nodes[2];	
//     printf("Dequeued element: %d\n", dequeue(f)->value); // Output: -1
//     printf("%d\n", getLength(f));
//     enqueue(f, &graph->nodes[0]);
//     printf("%d\n", getLength(f));
//     enqueue(f, &graph->nodes[1]);
//     enqueue(f, &graph->nodes[2]);
//     printf("Dequeued element: %d\n", dequeue(f)->value); // Output: 1
//     printf("%d\n", getLength(f));
//     printf("Dequeued element: %d\n", dequeue(f)->value); // Output: 2
//     printf("Dequeued element: %d\n", dequeue(f)->value); // Output: 3
//     printf("%d\n", getLength(f));
//     printf("Dequeued element: %d\n", dequeue(f)->value); // Output: -1
//     return 0;
// }