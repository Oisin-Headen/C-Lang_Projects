/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "game.h"
/**
 * This is the core of the game - it manages the game loop. This function 
 * should implement the algorithm specified under the "Play Game" assignment
 * specification.
 **/
struct player * play_game(struct player players[])
{
    gameboard board;
    struct player *current, *other;
    BOOLEAN continuing = TRUE;
    enum input_result player_input, move_outcome;
    enum game_state state_of_game;

    int first_player;

    player_input = player_init(&players[0], 1);
    if(player_input == IR_RTM)
    {
        return NULL;
    }
    player_input = player_init(&players[1], 2);
    if(player_input == IR_RTM)
    {
        return NULL;
    }
    current = &players[0];
    other = &players[1];

    /* Init the gameboard and randomize the game */
    gameboard_init(board);
    randomize_game(players, &first_player);

    /* Compares the current first player with the randomized first player
     * to see if they're the same. If they are not, 
     * the current player is swapped. 
     * The syntax of the players array address is a bit convoluted, 
     * but I thought it best to reference it as an array */
    if(!(&players[first_player] == current))
    {
        swap_players(&current, &other);
    }
    /* the game loop */
    while(continuing)
    {
        gameboard_display(board);
        state_of_game = is_game_over(
                      (const enum cell_contents(*)[BOARDWIDTH])board, current);
        switch(state_of_game)
        {
            case GS_CONTINUING:
                calc_score(board, current);
                move_outcome = player_turn(board, current);
                if(move_outcome == IR_RTM)
                {
                    return determine_winner(current, other);
                }
                swap_players(&current, &other);
                break;

            case GS_DRAW:
                continuing = FALSE;
                break;

            case GS_LOST:
                return determine_winner(current, other);
        }
    }
    /* neither player had a move possible */
    return determine_winner(current, other);
}
/* finds the winner based on the players' scores */
struct player* determine_winner(struct player *current, struct player * other)
{
    int final_score;
    /* if the scores are equal, there is no winner */
    if(current -> score == other -> score)
    {
        return NULL;
    }
    /* otherwise the player with the higher score has their score 
     * set to the difference, and is returned */
    else if(current -> score > other -> score)
    {
        final_score = current -> score - other -> score;
        current -> score = final_score;
        return current;
    }
    else
    {
        final_score = other -> score - current -> score;
        other -> score = final_score;
        return other;
    }
}

/**
 * randomises the colours for the two players. As white always goes first, the
 * index of the players array that holds the player with the white token will
 * be returned through the curplayer pointer. That is, it is what we call an 
 * "out" parameter. Don't forget to SEED the random number generator ...
 **/
void randomize_game(struct player players[], int * curplayer)
{
    int player_index;
    int counter;

    /* Both players are set to red at the start, 
     * then one of them is set to white */
    for(counter = 0; counter < NUM_PLAYERS; counter++)
    {
        players[counter].token_colour = CC_RED;
    }
    srand(time(NULL));
    player_index = rand()%2;
    players[player_index].token_colour = CC_WHITE;

    *curplayer = player_index;
}

/**
 * This function takes in pointers to the two player pointers used in play_game
 * and it swaps them. Some of you will find this requirement very hard. I do 
 * encourage you to work this through though as this technique is commonly used
 * in C. You may assume there will be a question on pointers to pointers on the
 * exam as well.
 **/
void swap_players(struct player ** current, struct player ** other)
{
    struct player *temp = *current;
    *current = *other;
    *other = temp;
}
