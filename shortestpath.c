// Dan Suciu
// ECE 368 Project 3
// 
// - Accepts 2 command line arguments, first the Filename of the map and second the Filename of the query in .txt format
// - Writes solution to command line in the format...
//
// (shortest distance)
// (list of vertices traversed)
//
// - Compile with:
// - gcc -Werror -Wall -O3 -lm shortestpath.c -o shortestpath
//
// - Example of run:
// ./shortestpath map.txt query.txt
//
// output is written to the command line

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "shortestpath.h"

//global scope variables
int numvert;
int numedge;
int numquery;
int startvert;
int endvert;

int main(int argc, char * * argv)
{
	int countvert, countedge, xcoord, ycoord, currentvert, currentneighbor, countrow, countcol, querycount;
	int counter;

	if (argc > 3)
	{//error checking
		fprintf(stderr, "\nERROR! TOO MANY INPUTS, shortestpath.c ACCEPTS 2 INPUTS: MAP.txt & QUERY.txt\n\n");
		return  (ERROR);
	}

	char * filename = argv[1];
	char * filename2 = argv[2];

	FILE * fp = fopen(filename, "r");
	FILE * fp2 = fopen(filename2, "r");

	if (fp == NULL || fp2 == NULL)
	{//error checking
		fprintf(stderr, "\nERROR! ONE OF THE FILES COULD NOT BE OPENED.\n\n");
		return (ERROR);
	}

	//scan in first line of files
	fscanf(fp, "%d %d\n", &numvert, &numedge);
	fscanf(fp2, "%d\n", &numquery);

	//create 2-D array for the vertice neighbors based on index
	int * * neighbors = (int * *) malloc(sizeof(int *) * numvert);
	//create 2-D array for the vertice coordinates based on index
	int * * coordinates = (int * *) malloc(sizeof(int *) * 2);
	coordinates[0] = (int *) malloc(sizeof(int) * numvert);
	coordinates[1] = (int *) malloc(sizeof(int) * numvert);

	fprintf(stdout, "\nallocating neighbor matrix memory...\n");
	for (counter = 0; counter < numvert; ++counter)
	{//initialize every column
		neighbors[counter] = (int *) malloc(sizeof(int) * numvert);
	}

	fprintf(stdout, "\nfilling coordinate matrix...\n");
	//fill the coordinate matrix
	for (countvert = 0; countvert < numvert; ++countvert)
	{
		fscanf(fp, "%d %d %d\n", &currentvert, &xcoord, &ycoord);
		coordinates[0][countvert] = xcoord;
		coordinates[1][countvert] = ycoord;
	}

	fprintf(stdout, "\ninitializing neighbor matrix...\n");
	//initialize neighbor matrix with -1's
	for (countrow = 0; countrow < numvert; ++countrow)
	{
		for (countcol = 0; countcol < numvert; ++countcol)
		{
			neighbors[countrow][countcol] = -1;
		}
	}

	fprintf(stdout, "\nfilling neighbor matrix...\n");
	//fill the neighbor matrix
	for (countedge = 0; countedge < numedge; ++countedge)
	{
		fscanf(fp, "%d %d\n", &currentvert, &currentneighbor);
		for (countrow = 0; neighbors[countrow][currentvert] != -1; ++countrow)
		{
			;
		}
		neighbors[countrow][currentvert] = currentneighbor;

		for (countrow = 0; neighbors[countrow][currentneighbor] != -1; ++countrow)
		{
			;
		}
		neighbors[countrow][currentneighbor] = currentvert;
	}

	fprintf(stdout, "\nbeginning queries...\n\n");
	for (querycount = 0; querycount < numquery; ++querycount)
	{//loop through queries and call dijkstra with every start vertex and end vertex
		fscanf(fp2, "%d %d\n", &startvert, &endvert);
		dijkstra(coordinates, neighbors, startvert, endvert);
	}

	//free memory and close file pointers
	for (counter = 0; counter < numvert; ++counter)
	{
		free(neighbors[counter]);
	}

	free(coordinates[0]);
	free(coordinates[1]);

	free(neighbors);
	free(coordinates);

	fclose(fp);
	fclose(fp2);

	return (OK);
}

void dijkstra(int * * coordinates, int * * neighbors, int startvert, int endvert)
{
	int count, dist, nextdist, currentneighbor, currentvert;
	int x1, y1, x2, y2;
	int * path = malloc(sizeof(int) * numvert);
	int * distances = malloc(sizeof(int) * numvert);
	int * visited = malloc(sizeof(int) * numvert);
	int * previous = malloc(sizeof(int) * numvert);

	for (count = 0; count < numvert; ++count)
	{//initialize all vertices to max distance, not visited, and invalid previous vertices
		distances[count] = INT_MAX;
		visited[count] = 0;
		previous[count] = -1;
	}
	//start at the start vertex and set its visited flag
	visited[startvert] = 1;
	//initialize the start vertex's distance to 0
	distances[startvert] = 0;
	//make the start vertex's previous itself
	previous[startvert] = startvert;
	//set the current vertex to be the starting vertex
	currentvert = startvert;

	for (nextdist = INT_MAX; visited[endvert] == 0; nextdist = INT_MAX)
	{//loop until we have visited the destination vertex
		for (count = 0; count < numvert; ++count)
		{//loop through all distances to find the smallest that is not visited
			if (distances[count] < nextdist && visited[count] == 0)
			{
				nextdist = distances[count];
				currentvert = count;
			}
		}
		//visit the smallest distance's vertex
		visited[currentvert] = 1;
		fprintf(stdout, "visiting %d\n\n", currentvert);
		for (count = 0; neighbors[count][currentvert] != -1; ++count)
		{//loop through the neighbors of the current vertex and retrieve their coordinates
			currentneighbor = neighbors[count][currentvert];
			x1 = coordinates[0][currentvert];
			y1 = coordinates[1][currentvert];
			x2 = coordinates[0][currentneighbor];
			y2 = coordinates[1][currentneighbor];
			//calculate distance
			dist = distances[currentvert] + distance(x1, y1, x2, y2);

			if (dist < distances[currentneighbor] && visited[currentneighbor] == 0)
			{//if the new distance is smaller, and we haven't visited the vertex yet, replace the distance
				distances[currentneighbor] = dist;
				previous[currentneighbor] = currentvert;
			}
		}
	}

	fprintf(stdout, "%d\n", distances[currentvert]);
	//write path
	for (count = 1, path[0] = currentvert; currentvert != startvert; ++count)
	{
		currentvert = previous[currentvert];
		path[count] = currentvert;
	}
	//print path
	for (count -= 1; count >= 0; --count)
	{
		fprintf(stdout, "%d ", path[count]);
	}
	fprintf(stdout, "\n\n");

	//free memory
	free(distances);
	free(visited);
	free(previous);
	free(path);

	return;
}

int distance(int x1, int y1, int x2, int y2)
{//returns distance between (x1, y1) and (x2, y2) as an integer
	int dist = (int) sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	return (dist);
}
