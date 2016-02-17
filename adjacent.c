// Dan Suciu
// ECE 368 Project 2
// 
// - Accepts 1 command line argument, being the Filename holding the map
//
// - Compile with:
// - gcc -Werror -Wall adjacent.c -o adjacent
//
// - Example of run:
// ./adjacent example.txt
//
// output is written to command line

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

/* Return/Error Codes */
#define OK               (  0 )  // No errors, everything as should be
#define ERROR            ( -1 ) // Generic error

int main(int argc, char * * argv)
{
	int numvert;
	int numedge;
	int count, countvert, countedge, countrow, countcol;
	int current, currentvert, currentneighbor;
	char * buffer = malloc(sizeof(char) * 10000);

	if (argc > 2)
	{//error checking
		fprintf(stderr, "\nERROR! TOO MANY INPUTS, adjacent.c ACCEPTS 1 INPUT: FILENAME.txt\n\n");
		return  (ERROR);
	}

	char * filename = argv[1];

	FILE * fp = fopen(filename, "r");

	if (fp == NULL)
	{//error checking
		fprintf(stderr, "\nERROR! FILE COULD NOT BE OPENED.\n\n");
		return (ERROR);
	}

	//scan in first line of file
	fscanf(fp, "%d %d", &numvert, &numedge);
	//get the newline
	fgetc(fp);
	//create 2-D array for the vertice neighbors based on index
	int neighbors[numedge][numvert];

	//initially fill 2-D array with -1's
	for (countrow = 0; countrow < numedge; ++countrow)
	{
		for (countcol = 0; countcol < numvert; ++countcol)
		{
			neighbors[countrow][countcol] = -1;
		}
	}

	//skip to the neighbor definitions
	for (count = 0; count < numvert; ++count)
	{
		fgets(buffer, 10000, fp);
	}

	//create neighbors
	for (countedge = 0, countrow = 0; countedge < numedge; countrow = 0, ++countedge)
	{
		fscanf(fp, "%d ", &currentvert);
		fscanf(fp, "%d", &currentneighbor);

		while (neighbors[countrow][currentvert] != -1)
		{
			++countrow;
		}

		neighbors[countrow][currentvert] = currentneighbor;
		//get newline
		fgetc(fp);
	}

	int dont = 0;
	//finish neighbors
	for (countrow = 0, countvert = 0, count = 0; countvert < numvert; dont = 0, countrow = 0, count = 0, ++countvert)
	{
		while (neighbors[countrow][countvert] != -1)
		{
			current = neighbors[countrow][countvert];
			
			while (neighbors[count][current] != -1)
			{
				if (neighbors[count][current] == countvert)
				{
					dont = 1;
				}
				++count;
			}
			if (dont == 0)
			{
				neighbors[count][current] = countvert;
			}
			++countrow;
		}
	}

	//print neighbors
	for (countvert = 0; countvert < numvert; countrow = 0, ++countvert)
	{
		fprintf(stdout, "%d: ", countvert);
		while (neighbors[countrow][countvert] != -1)
		{
			fprintf(stdout, "%d ", neighbors[countrow][countvert]);
			++countrow;
		}
		fprintf(stdout, "\n");
	}

	free(buffer);

	return (OK);
}