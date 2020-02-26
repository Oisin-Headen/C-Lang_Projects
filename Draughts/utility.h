/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "shared.h"
#include <limits.h>

#ifndef UTILITY_H
#define UTILITY_H
enum input_result
{
        IR_FAILURE, IR_SUCCESS, IR_RTM
};

/* The additional chars added to the end of all strings by fgets() */
#define EXTRA_CHARS 2

void read_rest_of_line(void);

enum menu_choice
{
    MC_PLAY_GAME, MC_DISPLAY_SCOREBOARD, MC_QUIT, MC_ERROR
};

#define MENU_OPTION_SIZE 1
#define PLAY_AGAIN_SIZE 1

BOOLEAN create_move(int, int, int, int, /* out */ struct move*);
enum menu_choice get_player_choice();
BOOLEAN get_play_again();
#endif /* UTILITY_H */
