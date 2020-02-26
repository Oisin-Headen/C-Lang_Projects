/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "shared.h"
#include "gameboard.h"
/* is_game_over() needs the create_move function in utility  */
#include "utility.h"

#ifndef GAMERULES_H
#define GAMERULES_H

/* the distances a piece is allowed to move */
#define NORMAL_MOVE_DIST 1
#define ATTACK_MOVE_DIST 2

#define INITIAL_MOD -1
#define END_MOD 2
#define MOD_CHANGE 2

#define INITIAL_PLAYER_SCORE 0

/**
 * the various states the game could be in which can be returned from
 * is_game_over()
 **/
enum game_state
{
        GS_CONTINUING, GS_LOST, GS_DRAW
};

struct player;

BOOLEAN is_valid_move(const gameboard, const struct move* , struct player*);
enum game_state is_game_over(const gameboard, struct player *); 
BOOLEAN test_player_moves(enum cell_contents, const gameboard);
BOOLEAN test_for_moves(int, int, const gameboard, enum cell_contents);
void calc_score(gameboard, struct player*);
/* both of these functions take a mess of parameters, so that is_valid_move()
 * can calculate and gather them all at once*/
BOOLEAN do_preliminary_checks(enum cell_contents, enum cell_contents, 
                              struct player *, int, int);
BOOLEAN do_attack_checks(int, int, struct player *, const gameboard, 
                                                        int, int, int, int);
#endif
