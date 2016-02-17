#ifndef __huff_h_
#define __huff_h_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Return/Error Codes */
#define OK               (  0 )  // No errors, everything as should be
#define ERROR            ( -1 ) // Generic error

/* Function Prototypes */
void dijkstra(int * * coordinates, int * * neighbors, int startvert, int endvert);
int distance(int x1, int y1, int x2, int y2);

#endif  // __huff_h_