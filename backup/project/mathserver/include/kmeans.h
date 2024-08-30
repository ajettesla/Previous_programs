#ifndef _KMEANS_H
#define _KMEANS_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_POINTS 4096
#define MAX_CLUSTERS 32


typedef struct point
{
    float x; // The x-coordinate of the point
    float y; // The y-coordinate of the point
    int cluster; // The cluster that the point belongs to
} point;


extern int k;      // number of centroids
extern point data[MAX_POINTS];		// Data coordinates
extern point cluster[MAX_CLUSTERS]; // The coordinates of each cluster center (also called centroid)

void read_data(char *argv);

int get_closest_centroid(int i, int k);

bool assign_clusters_to_points();

void update_cluster_centers();

int kmeans(int k);

void write_results(char *buffer);

void startkmeans(char *name,char* argv);

#endif

