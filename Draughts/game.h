/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "shared.h"
#include "gameboard.h"
#include "gamerules.h"
#include "player.h"
#include <time.h>

#ifndef GAME_H
#define GAME_H
struct player * play_game(struct player []);
struct player* determine_winner(struct player*, struct player*);
void randomize_game(struct player[], int * curplayer);
void swap_players(struct player ** current, struct player ** other);
#endif /* GAME_H */
