/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "utility.h"

#include "gameboard.h"

/**
 * this function is required for buffer clearing. It simply removes all 
 * characters from the input buffer. Note: you should only call this when
 * you detect buffer overflow. 
 **/
void read_rest_of_line(void)
{
        int ch;
        while ( ch = getc(stdin), ch != '\n' && ch != EOF)
                ;
}

/* used by the player module and the gamerules module */
BOOLEAN create_move(int src_y, int src_x, int dest_y, int dest_x, 
                                        /* out */ struct move *output_move)
{
    struct location start, end;
    
    /* validation of input, using gameboard #defines.
     * They are named in terms of rows, but the numbers are right for 
     * columns as well */
    if (src_x < TOP_ROW || src_x > BOTTOM_ROW)
    {
        return FALSE;
    }
    if (src_y < TOP_ROW || src_y > BOTTOM_ROW)
    {
        return FALSE;
    }
    if (dest_x < TOP_ROW || dest_x > BOTTOM_ROW)
    {
        return FALSE;
    }
    if (dest_y < TOP_ROW || dest_y > BOTTOM_ROW)
    {
        return FALSE;
    }

    start.x = src_x;
    start.y = src_y;
    end.x = dest_x;
    end.y = dest_y;
    output_move -> start = start;
    output_move -> end = end;
    return TRUE;
}

/* used by draughts.c main() to get a player choice */
enum menu_choice get_player_choice()
{
    char option[MENU_OPTION_SIZE + EXTRA_CHARS];
    int chosen_option;
    char *end;
    
    fgets(option, MENU_OPTION_SIZE + EXTRA_CHARS, stdin);

    if(option[strlen(option)-1]!='\n')
    {
        printf("Error: too much input\n");
        read_rest_of_line();
        return MC_ERROR;
    }

    option[strlen(option)-1] = 0;
    chosen_option = strtol(option, &end, 10);

    if(*end != '\0')
    {
        printf("Error: data entered was not numeric\n");
        return MC_ERROR;
    }

    switch(chosen_option)
    {
        case 1:
            return MC_PLAY_GAME;
            break;
        case 2:
            return MC_DISPLAY_SCOREBOARD;
            break;
        case 3:
            return MC_QUIT;
            break;
        default:
            printf("Error: that was not an option\n");
            return MC_ERROR;
    }
    /* there are many different errors that could occur, and this function
     * handles all of them, telling the caller that there was an error */
}

/* asks the user if they want to play again */
BOOLEAN get_play_again()
{
    char option[PLAY_AGAIN_SIZE + EXTRA_CHARS];

    while(TRUE)
    {
        printf("Would you like to play again? (y/n) ");
        fgets(option, PLAY_AGAIN_SIZE + EXTRA_CHARS, stdin);
        
        if(option[strlen(option)-1]!='\n')
        {
            printf("Error: too much input\n");
            read_rest_of_line();
            continue;
        }
        /* the case of the letter the user entered is ignored.
         * the break statements aren't needed because the function returns 
         * before it would overrun onto the other options */
        switch(option[0])
        {
            case 'y':
            case 'Y':
                return TRUE;
            case 'n':
            case 'N':
                return FALSE;
            default:
                printf("Error: that was not a valid option\n");
        }
    }



}
