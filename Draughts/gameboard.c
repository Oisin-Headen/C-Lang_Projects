/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "gameboard.h"
#include "player.h"
#include "gamerules.h"

/* The template for the starting board. This is an example of where a global
 * variable might come in handle. Note that since it is a static const 
 * it is only available within this file (that's what static means) and 
 * it is not modifiable (that's what const means).
 */
static const gameboard template_board = 
{
	{
		CC_RED, CC_EMPTY, CC_RED, CC_EMPTY, CC_RED, CC_EMPTY, CC_RED, 
		CC_EMPTY
	},
	{
		CC_EMPTY, CC_RED, CC_EMPTY, CC_RED, CC_EMPTY, CC_RED, CC_EMPTY, 
		CC_RED
	},
	{
		CC_RED, CC_EMPTY, CC_RED, CC_EMPTY, CC_RED, CC_EMPTY, CC_RED, 
		CC_EMPTY
	},
	{
		CC_EMPTY, CC_EMPTY, CC_EMPTY, CC_EMPTY, CC_EMPTY, CC_EMPTY, 
		CC_EMPTY, CC_EMPTY
	},
	{
		CC_EMPTY, CC_EMPTY, CC_EMPTY, CC_EMPTY, CC_EMPTY, CC_EMPTY, 
		CC_EMPTY, CC_EMPTY
	},
	{
		CC_EMPTY, CC_WHITE, CC_EMPTY, CC_WHITE, CC_EMPTY, CC_WHITE, 
		CC_EMPTY, CC_WHITE
	},
	{
		CC_WHITE, CC_EMPTY, CC_WHITE, CC_EMPTY, CC_WHITE, CC_EMPTY, 
		CC_WHITE, CC_EMPTY
	},
	{
		CC_EMPTY, CC_WHITE, CC_EMPTY, CC_WHITE, CC_EMPTY, CC_WHITE, 
		CC_EMPTY, CC_WHITE
	}
};

/**
 * simply copy the above template board into the board parameter passed into
 * this function.
 **/
void gameboard_init(gameboard board)
{
    memcpy(board, template_board, sizeof(gameboard));
}

/**
 * Display the game board. Your display should precisely match the provided
 * output example in the assignment specifications. I have provided colour
 * codes in the header file so you can easily output the colours of red and 
 * white.
 **/
void gameboard_display(gameboard board)
{
    int row, column;
    enum cell_contents space;
    /* The top numbers can be printed easily, done here with a constant*/
    printf(BOARD_TOP);
    printf(SEPARATOR);
    for(row = 0; row < BOARDHEIGHT; row++)
    {
        printf(" %d |", row+1);
        for(column = 0; column < BOARDWIDTH; column++)
        {
            space = board[row][column];
            switch(space)
            {
                case CC_EMPTY:
                    printf("   |"); 
                    break;

                case CC_RED:
                    printf(" %s%c%s |", RED_COLOUR, NORMAL_TOKEN, 
                                                            RESET_COLOUR);
                    break;

                case CC_WHITE:
                    printf(" %s%c%s |", WHITE_COLOUR, NORMAL_TOKEN, 
                                                            RESET_COLOUR);
                    break;

                case CC_K_RED:
                    printf(" %s%c%s |", RED_COLOUR, KING_TOKEN, RESET_COLOUR);
                    break;

                case CC_K_WHITE:
                    printf(" %s%c%s |", WHITE_COLOUR, KING_TOKEN, 
                                                            RESET_COLOUR);
                    break;
            }
        }
        printf(SEPARATOR);
    }
    printf("\n");
}

/**
 * this function modifies the gameboard based on the move passed in. It should 
 * validate that the move is valid and if not, return FALSE. It should then 
 * remove the token from it's starting point and place it in its ending point.
 * If this is an attack move, it should remove the piece taken from the board.
 * Finally if a piece has made it to the other end of the board, it should 
 * replace the normal piece with a KING piece of the same colour.
 **/
BOOLEAN apply_move(gameboard board, const struct move* move, 
		struct player* player)
{
/* this function does no validation, instead, it assumes that the is_valid_move
 * function already checked the validity */

    enum cell_contents moving_piece;
    BOOLEAN move_valid;
    int spaces_away;
    struct location enemy_location;

    move_valid = is_valid_move((const enum cell_contents(*)[BOARDWIDTH]) board,
                                                        move, player);
    if(!move_valid)
    {
        return FALSE;
    }
    moving_piece = board[move -> start.y][move -> start.x];


    board[move -> start.y][move -> start.x] = CC_EMPTY;
    board[move -> end.y][move -> end.x] = moving_piece;


    /* king making */
    if(player -> token_colour == CC_WHITE && move -> end.y == TOP_ROW)
    {
        board[move -> end.y][move -> end.x] = CC_K_WHITE;
    }
    /* If the player is not white, then they are red */
    else if(move -> end.y == BOTTOM_ROW)
    {
        board[move -> end.y][move -> end.x] = CC_K_RED;
    }
    /* for both of the above, the piece could already be a king, 
     * but that makes no difference */   

    /* attacking */
    spaces_away = move -> start.x - move -> end.x;
    if(spaces_away == 2 || spaces_away == -2)
    {
        /* since the move is valid (because of the is_valid_move call), 
         * the only way a piece could be 2 rows away is if it attacked. 
         *
         * This section finds the enemy by averaging the x and y positions 
         * of the start and end locations */
        enemy_location.x = (move -> start.x + move -> end.x)/2;
        enemy_location.y = (move -> start.y + move -> end.y)/2;
        board[enemy_location.y][enemy_location.x] = CC_EMPTY;
    }
    return TRUE;
}
