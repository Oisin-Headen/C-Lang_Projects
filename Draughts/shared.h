/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef SHARED_H
#define SHARED_H

#define NAMELEN 20

/* the boolean type - we use this to return TRUE or FALSE from a function. 
 * Note: given that TRUE is defined as not FALSE, you shouldn't depend 
 * of TRUE being 1. You can however depend of FALSE being 0
 */
typedef enum { FALSE, TRUE } BOOLEAN;

/* a location on the gameboard as x and y coordinates */
struct location
{
        int x, y;
};

/* a move which is really a start and and end location. So a move is moving 
 * from one location to another
 */
struct move
{
        struct location start;
        struct location end;
};

#endif
