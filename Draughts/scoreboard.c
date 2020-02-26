/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "scoreboard.h"

/**
 * initialise the scoreboard to an empty state. This means the names of all
 * the players should be an empty string.
 **/
void scoreboard_init(scoreboard board)
{
    int scoreboard_number = 0;

    for(; scoreboard_number < MAXNUMSCORES; scoreboard_number++)
    {
        strcpy(board[scoreboard_number].name, "");
        board[scoreboard_number].value=0;
    }
}

/**
 * adds the winner of the last game to the scoreboard. If you are not 
 * tracking scores, you should insert these sorted by name. However if you 
 * are tracking the scores, sort them the score value with the highest scores
 * earlier in the scoreboard array.
 *
 * Sorting involves searching for the insertion point in the array and then 
 * moving all elements below that element down by one. Finally you should insert
 * the new data.
 **/
BOOLEAN scoreboard_add(scoreboard board, struct player * winner)
{
    struct score new_score;
    int name_cmp_value, counter = 0;
    BOOLEAN continuing = TRUE;
    scoreboard temp_board;

    strcpy(new_score.name, winner -> name);
    new_score.value = winner -> score;

    while(continuing)
    {
        if(counter == MAXNUMSCORES)
        {
            continuing = FALSE;
        }
        else
        {
            /* sorts by scores, highest first */
            if(new_score.value > board[counter].value)
            {
                continuing = FALSE;
                continue;
            }
            /* if the scores are equal, then it sorts by name */
            if(new_score.value == board[counter].value)
            {
                name_cmp_value = strcmp(board[counter].name, new_score.name);
                /* if the name is empty, then counter is at a suitable location
                * if the cmp_value is greater than one, the new score name is 
                * higher than the old one, so it is at the right location */
                if(name_cmp_value > 0 || strcmp(board[counter].name, "") == 0)
                {
                    continuing = FALSE;
                    continue;
                }   
            }
            counter++;
        }
    }
    /* there was nowhere to put the new score */
    if(counter == MAXNUMSCORES)
    {
        return FALSE;
    }
    else
    {
        /* get the old values and positions */
        memcpy(temp_board, board, sizeof(temp_board));
        board[counter] = new_score;
        for(counter += 1; counter < MAXNUMSCORES; counter++)
        {
            /* move the scores down one space */
            board[counter] = temp_board[counter - 1];
        }
    }
	return TRUE;
}

/**
 * displays the scores in sorted order. You should use the output format as 
 * shown in the assignment specification as precisely as possible. Diversions
 * from that format will attract a deduction in marks. 
 **/
void scoreboard_display(scoreboard board)
{
    int counter;
    char name_space[NAMELEN];
    if(strcmp(board[0].name, "") == 0)
    {
        printf("The scoreboard is currenty empty.\n");
    }
    else
    {
        printf(START_SCOREBOARD);
        for(counter = 0; counter < MAXNUMSCORES; counter++)
        {
            if(strcmp(board[counter].name, "") != 0)
            {
                /* this adds the extra white space by copying a player's 
                 * name into a blank line */
                strcpy(name_space, EMPTY_SPACE);
                strncpy(name_space, board[counter].name, 
                            strlen(board[counter].name));
                printf(name_space);
                printf("|   %d\n", board[counter].value);
            }
        }
    }
}
