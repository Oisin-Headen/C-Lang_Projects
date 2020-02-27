/***********************************************************************
 * CPT220 - Programming in C
 * Study Period 4 2016 Assignment #2 Implementation
 * Full Name        : Oisin Headen
 * Student Number   : s3513821
 * Start up code provided by Paul Miller
 ***********************************************************************/
#include "picedit_line.h"

/************************************************************************
 * This file contains implementation of functions to manage the line struct
 * that contains a line from the file loaded into memory.
 ***********************************************************************/

/* allocating memory for a line and setting the values */
struct line *line_init(int line_num)
{
    struct line *line;

    line = malloc(sizeof(struct line));
    if(line == NULL)
    {
        perror("failed to allocate memory");
        return NULL;
    }
    else
    {
        line -> line = NULL;
        line -> length = 0;
        line -> line_num = line_num;
    }

    return line;
}

/* allocating the memory for a line */
void line_set_text(struct line *line, char *text)
{
    char *line_text;
    /* only allocates the minimum amount of space, the length of 
     * the string plus the nul terminator */
    line_text = malloc(sizeof(char) * (strlen(text)+1));
    if(line_text == NULL)
    {
        exit(EXIT_FAILURE);
    }
    strcpy(line_text, text);
    line -> length = strlen(line_text);

    line -> line = line_text;
}

void free_line(struct line *line)
{
    free(line -> line);
}
/* needed to renumber the lines */
void set_line_number(struct line *line, int line_num)
{
    line -> line_num = line_num;
}
