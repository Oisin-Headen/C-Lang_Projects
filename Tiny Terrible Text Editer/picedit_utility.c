/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #2 Implementation
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/

#include "picedit_utility.h"
#include "picedit_main.h"

/***********************************************************************
 * clears the input buffer from the keyboard if there has been buffer
 * overflow.
 **********************************************************************/
void read_rest_of_line(void)
{
        int ch;
        while(ch = getc(stdin), ch!='\n' && ch!=EOF)
                ;
        clearerr(stdin);
}

/*************************************************************************
 * initialize the datastructures used for the state of the system so 
 * safe values.
 ************************************************************************/
void picedit_init(struct picedit_system* picedit)
{
    /* passes a pointer to the line_list to the init function */
    line_list_init(&(picedit -> line_list));
    picedit -> current_file = NULL;
    picedit -> quit = FALSE;
}

/************************************************************************
 * loads the filename specified into the system. 
 ***********************************************************************/
BOOLEAN picedit_load(struct picedit_system* picedit, const char * fname)
{
    /* BUFSIZ is a constant provided in stdio.h to represent a very
     * large buffer. We use that instead of having to guess a maximum
     * line length 
     */
    char line[BUFSIZ + EXTRA_CHARS];
    enum list_return add_succeed;
    /* placeholder return value  - remove it and this comment and 
     * replace it with the logic for this function
     */
    FILE *file;
    file = fopen(fname, READ_MODE);
    
    if(file == NULL)
    {
        /* if the file doesn't exist, set the current file anyway, 
         * to be created later when saving */
        if(errno == ENOENT)
        {
            set_file(picedit, fname);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    } 
    set_file(picedit, fname);
    /* loops though the file  */
    while(fgets(line, BUFSIZ + EXTRA_CHARS, file) != NULL)
    {
        /* remove the newlines */
        if(line[strlen(line)-1] != '\n')
        {
            read_rest_of_line();
            printf("Error, file had a line that was too long\n");
            return FALSE;
        }
        line[strlen(line)-1] = 0;
        /* append the new line */
        add_succeed = list_add(&(picedit -> line_list), line, 
                                picedit -> line_list.num_lines+1);
        if(add_succeed == LR_MALLOC_FAILED)
        {
            /* malloc failed, so free as much as possible and then quit */
            picedit_free(picedit);
            exit(EXIT_FAILURE);
        }
        
    }
    picedit -> line_list.current_line = picedit -> line_list.first_line;
    printf("Read %d lines from %s\n", picedit -> line_list.num_lines, fname);
    fclose(file);
    return TRUE;
}

/* change the current file */
void set_file(struct picedit_system *picedit, const char *fname)
{
    char *temp_cur_file = malloc(sizeof(char) * (strlen(fname)+1));
    if(temp_cur_file == NULL)
    {
        picedit_free(picedit);
        exit(EXIT_FAILURE);
    }
    strcpy(temp_cur_file, fname);
    free((char *) picedit -> current_file);
    picedit -> current_file = temp_cur_file;
}

/**************************************************************************
 * saves the current picedit document back to the file it was read from.
 *************************************************************************/
BOOLEAN picedit_save(struct picedit_system* picedit)
{
    /* save the file to the current file*/
    if(!picedit_save_to_file(picedit, picedit -> current_file))
    {
        perror("Error saving");
        return FALSE;
    }
    return TRUE;
}

BOOLEAN picedit_save_to_file(struct picedit_system *picedit, const char *fname)
{
    FILE *file;
    int lines_saved;
    file = fopen(fname, WRITE_MODE);

    if(file == NULL)
    {
        return FALSE;
    }
    /* save the file to the specified file name */
    lines_saved = list_save(&(picedit -> line_list), file);
    printf("Successfully saved %d lines to %s\n", lines_saved, fname);
    fclose(file);
    return TRUE;
}

/**************************************************************************
 * frees any data allocated to the system.
 *************************************************************************/
void picedit_free(struct picedit_system* picedit)
{
    /* loop through the line_list */
    while(picedit ->line_list.num_lines > 0)
    {
        /* will delete the first index until there are no more nodes */
        list_delete(&(picedit -> line_list), 1);
    }
}

