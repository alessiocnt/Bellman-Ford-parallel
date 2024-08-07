#ifndef UTILS_H
	#define UTILS_H
    # define INIT 0
    struct Graph *importGraphFromFile(const char *filename);
    void printGraphToFile(int **matrix, int V, int E, char *filename);
    int computeThreadSeries(int N, int** series);
#endif