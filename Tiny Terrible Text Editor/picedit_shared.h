/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #2 Implementation
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/

#include <assert.h>

#ifndef SHARED_H
#define SHARED_H
/* the boolean type that can be used throughout the program */
typedef enum {FALSE, TRUE} BOOLEAN;
/* the maximum length of each line in the file we edit. */
#define LINE_LEN 70
#define NO_DELIMITER 1
#define DELIMITED_RANGE 2
/* the assumed width of the terminal window */
#define SCREENWIDTH 80
/* the assumed height of the terminal window */
#define SCREENHEIGHT 25

/* most functions in picedit allow entry of a 
 * range over which they operate. This data structure
 * will allow you to capture that range to pass into
 *  functions to implement this functionality.
 */
struct range
{
        int begin, end;
};

#endif
