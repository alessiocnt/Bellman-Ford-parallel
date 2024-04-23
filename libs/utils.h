#ifndef UTILS_H
	#define UTILS_H
    # define INIT 0     // INT_MAX
    double gettime(void);
    struct Graph *importGraphFromFile(const char *filename);
    void freeDevice(struct Graph* d_graph, int* d_dist, int* d_isNegativeCycle);
    void freeHost(struct Graph* h_graph, int* h_dist)
#endif