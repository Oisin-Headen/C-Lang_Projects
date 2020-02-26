/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #2 Implementation
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "picedit_shared.h"

#ifndef PICEDIT_LINE_H
#define PICEDIT_LINE_H
/**
 * contains the data for each node. Note that this contains both the 
 * string for the current line and the length of that line the value
 * returned for strlen for that line 
 **/
struct line
{
        /* dynamic string managed as part of this struct */
        char * line;
        /* the length of the string not counting the NUL
         * terminator
         */
        int length;
        /* the line number of this line within the text file */
        int line_num;
};

struct line *line_init(int);
void free_line(struct line *);
void line_set_text(struct line *, char *);
void set_line_number(struct line *, int);

#endif /* PICEDIT_LINE_H */
