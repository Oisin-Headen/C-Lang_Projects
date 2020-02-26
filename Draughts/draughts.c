/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "draughts.h"


/**
 * The starting point for the application. You should have a loop where you
 * prompt the user of your program what their menu choice and based on 
 * their choice, call play_game() to start a game, call display_scores()
 * to display the scoreboard or quit the program.
 **/
int main(void)
{
    /* the scoreboard that keeps track of all the winners of games 
     * played
     */
    scoreboard scboard;

    enum menu_choice user_choice;
    BOOLEAN play_again, added_to_scoreboard;

    /* the players to play the game */
    struct player players[NUM_PLAYERS],
                  /* a pointer to the winner of the last game played */
                  *winner;

    /* initialise the scoreboard to empty */
    scoreboard_init(scboard);

	/* loop until the user decided to quit */
    while (TRUE)
    {
        /* display menu */
        printf(MENU);
        /* calls an input function in utility.c */
        user_choice = get_player_choice();

        /* perform the user's choice */
        switch(user_choice)
        {
            case MC_PLAY_GAME:
				/* play the game. Uses a while loop to check if 
                 * the player wants to play again after a game */
                play_again = TRUE;
                while(play_again)
                {
                    winner = play_game(players);
                    /* if there was a winner, add them to the 
                     * scoreboard */
                    if(winner!=NULL)
                    {
                        added_to_scoreboard = scoreboard_add(scboard, winner);
                        /* notifies the user if they were added to the board */
                        if(added_to_scoreboard)
                        {
                            printf("%s added to scoreboard\n", winner -> name);
                        }
                    }
                    play_again = get_play_again();
                }
                break;

            case MC_DISPLAY_SCOREBOARD:
                /* display the scoreboard */
                scoreboard_display(scboard);
                break;

            case MC_QUIT:
                /* quit the game */
                return EXIT_SUCCESS;
                break;

            case MC_ERROR:
                continue;
        }
    }
    /* we should never get here */
}
