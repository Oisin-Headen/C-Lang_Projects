/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "shared.h"

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#define BOARDWIDTH 8
#define BOARDHEIGHT BOARDWIDTH
#define CH_BOARD_WIDTH 36
#define NORMAL_TOKEN 'o'
#define KING_TOKEN 'O'
#define RED_COLOUR "\x1B[0;31m"
#define WHITE_COLOUR "\x1B[0;37m"
#define RESET_COLOUR "\x1B[0m"
#define TOP_ROW 0
#define BOTTOM_ROW 7

#define BOARD_TOP   "   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |"
#define SEPARATOR "\n------------------------------------\n"

/* The contents of each cell on the game board will be one of these
 * values
 */
enum cell_contents
{
        CC_EMPTY, CC_RED, CC_WHITE, CC_K_RED, CC_K_WHITE
};

/* we typedef the gameboard array so "gameboard" is really just an 
 * alias for a 2-d array (8x8) of enum cell_contents
 */
typedef enum cell_contents gameboard[BOARDHEIGHT][BOARDWIDTH] ;
/* we predeclare the struct player so that we can have pointers to struct
 * player as parameters in the functions declared in this header file 
 */
struct player;
void gameboard_init(gameboard);
void gameboard_display(gameboard);
BOOLEAN apply_move(gameboard, const struct move*, struct player*);
#endif /* GAMEBOARD_H */
