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
#include "utility.h"
#ifndef PLAYER_H
#define PLAYER_H
/* the number of players in a game */
#define NUM_PLAYERS 2

#define COLOUR_STRING_LENGTH 6
#define MOVE_FORMAT "\nPlease enter a move in the form of x1,y1-x2,y2: "
#define MOVE_LENGTH 7
#define NUM_TOKENS 4
#define SRC_X 0
#define SRC_Y 1
#define DEST_X 2
#define DEST_Y 3


/* we reuse the cell contents enumeration to mean the colour of the tokens
 * for a player. I try to avoid typedefs but I believe in this context it makes
 * sense.
 */
typedef enum cell_contents colour;

/* the player in a game - they have a name, a score and a token colour. Thes
 * should be initialised in player_init()
 */
struct player
{
        char name[NAMELEN + 1];
        int score; /* count of number of pieces on the board */
        colour token_colour;   
};

enum input_result player_init(struct player *, int);
enum input_result player_turn(gameboard, struct player *);
BOOLEAN get_move_from_string(char [], struct move*);

#endif
