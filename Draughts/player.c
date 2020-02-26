/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "player.h"

/**
 * this function should initialise the player's data to default values (all 0)
 * and ask the player for their name. You should save their name to the player
 * struct. Note: I have added a playernum variable here so that you can 
 * distinguish between players. Please see the sample output in the assignment
 * specifications for expected behaviour here.
 **/
enum input_result player_init(struct player * newplayer, int playernum)
{
    char player_name[NAMELEN + EXTRA_CHARS];
    char *rtm_error;

    while (TRUE)
    {
        printf("Please enter the name of player %d: ", playernum);

        rtm_error = fgets(player_name, NAMELEN + EXTRA_CHARS, stdin);
        
        if((rtm_error == NULL) || (strcmp(player_name, "\n") == 0))
        {
            return IR_RTM;
        }

        if(player_name[strlen(player_name)-1]!='\n')
        {
            printf("Error: that name is too long\n");
            read_rest_of_line();
            continue;
        }
        /* removing the newline character */
        player_name[strlen(player_name)-1] = '\0';

        strcpy(newplayer -> name, player_name);
        /* score is initialized as well */
        newplayer -> score = 0;
        return IR_SUCCESS;
    }
    /* this line is only to keep the compiler happy, we'll never get here */
    return IR_FAILURE;
}

/**
 * This function handles each player's turn. In each turn, you should display
 * whose turn it is, their color and if you are implementing score tracking
 * the number of tokens they currently have on the board. You should then 
 * prompt them to enter their turn. You should then tokenize their input and
 * store the information in a struct move. This should then be passed into
 * apply_move() which will do the work of validating and applying the move.
 *
 * Note: it is your responsibility to ensure all operations and successful and
 * notify the user if they are not.
 **/
enum input_result player_turn(gameboard board, struct player * player)
{
    char colour[COLOUR_STRING_LENGTH];
    char move_string[MOVE_LENGTH + EXTRA_CHARS];

    char *rtm_error;
    struct move new_move;
    BOOLEAN valid_move, tokenize_success;

    if(player -> token_colour == CC_WHITE)
    {
        strcpy(colour, "white");
    }
    else
    {
        strcpy(colour, "red");
    }
    /* validating input until the user enters it correctly */
    while(TRUE)
    {
        printf("It is %s's turn and their tokens are %s in colour.\n", 
                                                    player -> name, colour);

        printf("They currently have %d tokens.", player -> score);
        printf(MOVE_FORMAT);
        rtm_error = fgets(move_string, MOVE_LENGTH + EXTRA_CHARS, stdin);
        if((rtm_error == NULL) || (strcmp(move_string, "\n") == 0))
        {
            return IR_RTM;
        }
        if(move_string[strlen(move_string)-1]!='\n')
        {
            printf("Error: too much input was entered\n");
            read_rest_of_line();
            continue;
        }
        tokenize_success = get_move_from_string(move_string, &new_move);
        if(tokenize_success == FALSE)
        {
            printf("Error: the move you entered was not valid\n");
            continue;
        }
        valid_move = apply_move(board, &new_move, player);
        if(!valid_move)
        {
            printf("Error: That was not a valid move\n");
            continue;
        }
        else
        {
            return IR_SUCCESS;
        }
    }
    return IR_RTM;
}

/* function to turn a string into a move */
BOOLEAN get_move_from_string(char move_string[],
                                    /* out */struct move *output_move)
{
    int positions[NUM_TOKENS];
    int counter;
    BOOLEAN move_valid;
    char *tokens[NUM_TOKENS];
    char *end;

    tokens[SRC_X] = strtok(move_string, ",");
    tokens[SRC_Y] = strtok(NULL, "-");
    tokens[DEST_X] = strtok(NULL, ",");
    tokens[DEST_Y] = strtok(NULL, "\n");

    for(counter = 0; counter < NUM_TOKENS; counter++)
    {
        positions[counter] = strtol(tokens[counter], &end, 10);
        if (*end != '\0')
        {
            /* user has made a mistake */
            return FALSE; 
        }
        /* subtract one from user input, computer uses 0-7, human sees 1-8 */
        positions[counter]--;
    }

    /* parameters are passed in oddly to conform to sample
     * also, this is the more likely way a user will enter moves 
     * (top number, side number) */
    move_valid = create_move(positions[SRC_Y], positions[SRC_X], 
                             positions[DEST_Y], positions[DEST_X], 
                             output_move);
    if(!move_valid)
    {
        return FALSE;
    }

    return TRUE;
}
