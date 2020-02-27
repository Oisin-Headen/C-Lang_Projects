/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #2 Implementation
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/

#include "picedit_shared.h"
#include "picedit_line_list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

#ifndef PICEDIT_UTILITY_H
#define PICEDIT_UTILITY_H
/* newline and nul terminator at the end of each string read in with 
 * fgets()
 */
#define EXTRA_CHARS 2
/* each time we grow the token vector we double it in size */
/* required for strtol() when converting a string containing a decimal
 * integer to a long integer
 */
#define DECIMAL 10

#define READ_MODE "r"
#define WRITE_MODE "w"

/* The various results possible from input functions that read from the 
 * keyboard. SUCCESS, FAILURE, RTM (pressing enter on a new line or pressing
 * ctrl-d on a new line 
 */
enum input_result
{
        /* the input read failed */
        IR_FAILURE, 
        /* the input read was successful */
        IR_SUCCESS, 
        /* the user pressed ctrl-d or enter on a new line which cancels
         * the current operation
         */
        IR_RTM
};

/* forwards declaration of picedit_system: this allows us to have pointers
 * to struct picedit_system and only include the header that defines the 
 * type in the .c file. This avoids the problem of the recursive include
 */
struct picedit_system;

/* buffer clearing function */
void read_rest_of_line(void);

/* functions for manipulating the picedit type that representents the 
 * overall state of the system
 **/
void picedit_init(struct picedit_system*);
BOOLEAN picedit_load(struct picedit_system*, const char *);
BOOLEAN picedit_save(struct picedit_system*);
void picedit_free(struct picedit_system*);
BOOLEAN picedit_save_to_file(struct picedit_system*, const char *);
void set_file(struct picedit_system*, const char *);

#endif
