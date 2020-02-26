/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #2 Implementation
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/

#include "picedit_line_list.h"
#include "picedit_utility.h"

/***********************************************************************
 * contains the functions you will implement for the manipulation of the 
 * line list that contains the document loaded in from memory.
 **********************************************************************/

void line_list_init(struct line_list *list)
{
    /* don't need to allocate space for a line_list, 
     * it is already allocated in main */
    list -> first_line = NULL;
    list -> last_line = NULL;
    list -> current_line = NULL;
    list -> num_lines = 0;
}

enum list_return list_add(struct line_list *list, char *text, int index)
{
    struct line_node *new_node, *cur_node;
    struct line *line;
    BOOLEAN done = FALSE;
    int cur_index;
    /* checking that index is valid */
    if(index < 1 || index > list -> num_lines + 1)
    {
        return LR_FAIL;
    }

    /* if we are appending, we'll just start at the end */
    if(index == list -> num_lines + 1)
    {
        cur_index = list -> num_lines + 1;
        cur_node = NULL;
    }
    else
    {
        /* otherwise we'll go through from the start */
        cur_index = 1;
        cur_node = list -> first_line;
    } 

    while(!done)
    {
        if(cur_index == index)
        {
            new_node = node_init();
            line = line_init(list -> num_lines + 1);
            if(line == NULL || new_node == NULL)
            {
                /* malloc failed */
                return LR_MALLOC_FAILED;
            }
            line_set_text(line, text);
            new_node -> data = line;

            if(list -> last_line == NULL && list -> first_line == NULL)
            {
                /* new node is the only item */
                list -> first_line = new_node;
                list -> last_line = new_node;
                new_node -> prev = NULL;
                new_node -> next = NULL;
            }
            else if(cur_node == NULL)
            {
                /* new node is the last item */
                new_node -> next = NULL;
                new_node -> prev = list -> last_line;
                list -> last_line -> next = new_node;
                list -> last_line = new_node;
            }
            else if(cur_node -> prev == NULL)
            {
                /* new node is the first item */
                new_node -> next = cur_node;
                new_node -> prev = NULL;
                list -> first_line -> prev = new_node;
                list -> first_line = new_node;
            }
            else
            {
                /* adding the new node to the middle of the list */
                new_node -> next = cur_node;
                new_node -> prev = cur_node -> prev;
                cur_node -> prev -> next = new_node;
                cur_node -> prev = new_node;
            }
            list -> num_lines++;
            new_node -> data -> line_num = index;
            /* current_line is the last line inserted, 
             * also meaning it is the last line loaded */
            list -> current_line = new_node;
            done = TRUE;
        }
        else
        {
            cur_index++;
            cur_node = cur_node -> next;
        }
    }
    renumber_line_nums(list, index, INSERT_CHANGE);
    return LR_SUCCESS;
}

struct line_node * node_init()
{
    /* allocating space for node */
    struct line_node *node;
    node = malloc(sizeof(struct line_node));
    if(node == NULL)
    {
        return NULL;
    }
    else
    {
        node -> next = NULL;
        node -> prev = NULL;
        node -> data = NULL;
    }
    return node;
}

/* helper function to save the list to a file */
int list_save(struct line_list *list, FILE *file)
{    
    struct line_node *node = list -> first_line;
    int lines_saved = 0;
    while(node != NULL)
    {
        fputs(node -> data -> line, file);
        /* putting the newlines back */
        fputc('\n', file);
        node = node -> next;
        lines_saved++;
    }
    return lines_saved;
}

enum list_return list_delete(struct line_list *list, int index)
{
    struct line_node *node = list -> first_line;
    struct line_node *next_node, *prev_node;
    BOOLEAN done, is_current_line;

    done = FALSE;
    while(!done)
    {
        if(node == NULL)
        {
            /* the list did not contain a node of that index */
            return LR_FAIL;
        }
        else if(node -> data -> line_num == index)
        {
            is_current_line = (list -> current_line == node);
            /* removing this node from the list */
            next_node = node -> next;
            prev_node = node -> prev;
            if(prev_node != NULL)
            {
                prev_node -> next = next_node;
            }
            else
            {
                /* if there is no prev node, 
                 * then we're at the start of the list */
                list -> first_line = next_node;
            }
            if(next_node != NULL)
            {
                next_node -> prev = prev_node;
            }
            else
            {
                /*if there is no node next, then we're at the end of the list*/
                list -> last_line = prev_node;
            }
            list -> num_lines--;
            /* free the malloc'ed sections of this node */
            free_line(node -> data);
            free(node -> data);
            free(node);
            /* renumber the remaining lines */
            renumber_line_nums(list, index-1, DELETE_CHANGE);
            /* if the current line was deleted, 
             * then set the current line to the next line */
            if(is_current_line)
            {
                list -> current_line = next_node;
            }
            done = TRUE;
            
        }
        else
        {
            node = node -> next;
        }
    }
    return LR_SUCCESS;
}

void renumber_line_nums(struct line_list *list, int after_index, int change)
{
    BOOLEAN done = FALSE;
    struct line_node *node = list -> first_line;
    int cur_index = 1;
    while(!done)
    {
        if(node == NULL)
        {
            done = TRUE;
        }        
        else
        {
            if(cur_index > after_index)
            {
                /* will add or subtract from the line number, 
                 * depending on what argument was passed in */
                node -> data -> line_num += change;
            }    
            cur_index++;
            node = node -> next;
        }
    }
}

/* loops through the lines searching for an index */
struct line *get_line(struct line_list *list, int line_num)
{
    struct line_node *node = list -> first_line;
    BOOLEAN done = FALSE;
    while(!done)
    {
        if(node == NULL)
        {
            done = TRUE;
        }
        else if(node -> data -> line_num == line_num)
        {
            return node -> data;
        }
        else
        {
            node = node -> next;
        }
    }
    return NULL;
}

/* the print command changes the current line */
void set_current_line(struct line_list *list, int line_num)
{
    struct line_node *node = list -> first_line;
    if(get_line(list, line_num) == NULL)
    {
        return;
    }
    while(node -> data -> line_num != line_num)
    {
        node = node -> next;
    }
    list -> current_line = node;
}
