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
            ptrNode = &nullNode;
        } else {
            struct Node node = f->nodes[f->front];
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