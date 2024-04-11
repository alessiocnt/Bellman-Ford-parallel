// #include <stdio.h>
// #include <stdlib.h>
// #include "frontier.h"
// #include "graph.h"
// #include "graph.c"

// struct Frontier* createFrontier(int capacity) {
//     struct Frontier* frontier = (struct Frontier*)malloc(sizeof(struct Frontier));
//     frontier->capacity = capacity;
//     frontier->front = -1;
//     frontier->rear = -1;
//     frontier->nodes = (struct Edge*)malloc(capacity * sizeof(struct Edge));
//     return frontier;
// }

// int isEmpty(struct Frontier* f) {
//     return f->front == -1;
// }

// int isFull(struct Frontier* f) {
//     return (f->rear + 1) % f->capacity == f->front;
// }

// void enqueue(struct Frontier* f, struct Edge node) {
//     #pragma omp critical {
//         if (isFull(f)) {
//             printf("Frontier is full.\n");
//             return;
//         }
//         if (isEmpty(f)) {
//             f->front = 0;
//             f->rear = 0;
//         } else {
//             f->rear = (f->rear + 1) % f->capacity;
//         }
//         f->nodes[f->rear] = node;
//     }
// }

// struct Edge dequeue(struct Frontier* f) {
//     struct Edge node;
//     #pragma omp critical {
//         if (isEmpty(f)) {
//             struct Edge nullEdge = {-1, -1, -1};
//             return nullEdge;
//         }
//         node = f->nodes[f->front];
//         if (f->front == f->rear) {
//             f->front = -1;
//             f->rear = -1;
//         } else {
//             f->front = (f->front + 1) % f->capacity;
//         }
//     }
//     return node;
// }

// int main() {
//     struct Frontier* f = createFrontier(5); // Set initial capacity to 5

//     int V = 3; // Number of vertices in graph
// 	int E = 3; // Number of edges in graph
// 	struct Graph* graph = createGraph(V, E);

// 	// add edge 0-1
// 	graph->edge[0].src = 0;
// 	graph->edge[0].dest = 1;
// 	graph->edge[0].weight = -1;
// 	// add edge 0-2
// 	graph->edge[1].src = 0;
// 	graph->edge[1].dest = 2;
// 	graph->edge[1].weight = 4;
// 	// add edge 1-2
// 	graph->edge[2].src = 1;
// 	graph->edge[2].dest = 2;
// 	graph->edge[2].weight = 3;

//     printf("Dequeued element: %d\n", dequeue(f).src); // Output: 1
//     enqueue(f, graph->edge[0]);
//     enqueue(f, graph->edge[1]);
//     enqueue(f, graph->edge[2]);

//     printf("Dequeued element: %d\n", dequeue(f).src); // Output: 1
//     printf("Dequeued element: %d\n", dequeue(f).src); // Output: 2
//     printf("Dequeued element: %d\n", dequeue(f).src); // Output: 3

//     return 0;
// }
