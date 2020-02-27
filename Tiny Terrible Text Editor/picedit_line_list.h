/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #2 Implementation
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "picedit_shared.h"
#include "picedit_line.h"

/* contains the definition of the linked list we will manipulate */
#ifndef LINE_LIST_H
#define LINE_LIST_H

#define DELETE_CHANGE -1
#define INSERT_CHANGE 1
#define MIDDLE 2

/** 
 * a node for the list. note that we have separated out the data from 
 * the node structure. This is good practice as it means that our 
 * data is separated from out list structure and it is much easier 
 * to adapt our list to a different application - just change the 
 * data struct. You should notice that our list has pointers to previous
 * and next as it is a doubly linked list. You should ensure all links
 * are kept up to date on each modification. 
 **/
struct line_node
{
        /* a pointer to the line for this node in the linked 
         * list */
        struct line * data;
        /* a pointer to the next node */
        struct line_node * next;
        /* a pointer to the previous node */
        struct line_node * prev;
};

/**
 * the list structure itself. This is the header structure that is passed
 * around so that we treat the whole list as a single data structure.
 **/
struct line_list
{
        /* a pointer to the beginning of the list */
        struct line_node * first_line;
        /* a pointer toe the end of the list */
        struct line_node * last_line;
        /* a pointer to the current line in the list */
        /* It's important to realise that current_line is not an "owning
         * pointer" in the sense that it just points to memory owned by 
         * another pointer
         */
        struct line_node * current_line;
        /* how many lines are there in this document ? */
        int num_lines;
};
/* special enum to distiguish between a fail and a malloc fail */
enum list_return
{
    LR_FAIL, LR_SUCCESS, LR_MALLOC_FAILED 
};

/* you'll need to propose and implement functions for the manipulating of 
 * the line_list type */

void line_list_init(struct line_list*);
enum list_return list_add(struct line_list*, char*, int);
struct line_node* node_init();
int list_save(struct line_list*, FILE*);
enum list_return list_delete(struct line_list*, int);
struct line* get_line(struct line_list*, int);
void renumber_line_nums(struct line_list*, int, int);
void set_current_line(struct line_list*, int);

#endif
