/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #2 Implementation
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/

#include "picedit_line_list.h"
#include "picedit_utility.h"
#include "picedit_command.h"

#ifndef PICEDIT_MAIN_H
#define PICEDIT_MAIN_H

#define NO_ARGS 1
#define ONE_ARG 2
#define FILENAME_ARG 1

/**
 * the header structure that is passed around to represent the current 
 * state of the system. It has a line_list that holds the file data, 
 * a char pointer that holds the name of the current file loaded so we 
 * can save back to it and a BOOLEAN quit which represents whether the 
 * user has decided to quit the program.
 **/
struct picedit_system
{
        /* the line_list that holds all the lines for the document 
         * loaded into memory. Please note that this is NOT a pointer!
         * Think about what this means in terms of memory allocatoin.
         */
        struct line_list line_list;
        /* the name of the file currently loaded into memory */
        const char * current_file;
        /* has the user chosen to quit ? */
        BOOLEAN quit;
};

#endif
