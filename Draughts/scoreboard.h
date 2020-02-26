/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "shared.h"
#include "player.h"

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#define MAXNUMSCORES 10

#define START_SCOREBOARD "\nDraughts - List of Winners\n"\
                           "============================="\
                           "=============================\n"\
                           "Name                |Score\n"\
                           "----------------------------\n"
#define EMPTY_SPACE "                    "

struct score
{
        char name[NAMELEN + 1];
        int value;
};

typedef struct score scoreboard[MAXNUMSCORES];

void scoreboard_init(scoreboard);
BOOLEAN scoreboard_add(scoreboard, struct player *);
void scoreboard_display(scoreboard);
#endif /* SCOREBOARD_H */
