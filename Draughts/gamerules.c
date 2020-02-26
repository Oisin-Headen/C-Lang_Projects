/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #1 
* Full Name        : Oisin Headen
* Student Number   : s3512821
* Start up code provided by Paul Miller
***********************************************************************/
#include "gamerules.h"
#include "player.h"

/**
 * tests whether the move passed in is valid given the current board state and
 * given that the player pointer passed in points to the current player. You 
 * should implement the testing criteria (on only those criteria) mentioned
 * for this requirement in the assignment specifications. 
 **/
BOOLEAN is_valid_move(const gameboard board, const struct move* move, 
                struct player* player)
{
    /* important values needed later */
    enum cell_contents start_space = board[move -> start.y][move -> start.x];
    enum cell_contents end_space = board[move -> end.y][move -> end.x];

    int start_x = move -> start.x;
    int start_y = move -> start.y;
    int end_y = move -> end.y;
    int end_x = move -> end.x;


    BOOLEAN preliminary_checks, attack_checks;

    int x_distance = abs(start_x - end_x), y_distance = abs(start_y - end_y);

    /* starting checks */
    preliminary_checks = do_preliminary_checks(start_space, end_space, 
                                                player, start_y, end_y);
    if(!preliminary_checks)
    {
        /* the starting checks failed, so the move is invalid */
        return FALSE;
    }

    /* if the piece is just moving to an adjacent space, 
     * then the move is valid */
    if(x_distance == NORMAL_MOVE_DIST && y_distance == NORMAL_MOVE_DIST)
    {
        return TRUE;
    }

    /* attack checks */
    attack_checks = do_attack_checks(x_distance, y_distance, player, board,
                                        start_x, start_y, end_x, end_y);
    if(attack_checks)
    {
        /* the attack is possible */
        return TRUE;
    }

    /* all possible correct moves are covered above, 
     * anything else will fail */
    return FALSE;
}

/**
 * this function tests whether the current player has lost the game. A game is
 * lost when a player has no further legal moves they can make. You need to 
 * test for legal moves in all directions. You may find a helper function makes
 * this code much easier to write.
 **/
enum game_state is_game_over(const gameboard board, struct player * player)
{
    BOOLEAN can_make_move;
    enum cell_contents player_colour = player -> token_colour;

    can_make_move = test_player_moves(player_colour, board);
    /* if the player can make a move, then the game continues */
    if(can_make_move)
    {
        return GS_CONTINUING;
    }
    else
    {
        /* if the player can't make a move, then this checks 
         * whether the other player can make a move */
        if(player_colour == CC_WHITE)
        {
            can_make_move = test_player_moves(CC_RED, board);
        }
        else
        {
            can_make_move = test_player_moves(CC_WHITE, board);
        }
        if(can_make_move)
        {
            /* if the other player can move, then this player loses */
            return GS_LOST;
        }
        else
        {
            /* if the other player cannot move, the game is a draw */
            return GS_DRAW;
        }
    }
} 

/* first helper function for is_game_over(). this checks 
 * whether a player has a move they can make */
BOOLEAN test_player_moves(colour player_colour, const gameboard board)
{
    int row, column;
    BOOLEAN valid_move;

    for(row = 0; row < BOARDHEIGHT; row++)
    {
        for(column = 0; column < BOARDWIDTH; column++)
        {
            /* for each space on the board, 
             * this checks if the player has a piece there */
            if(player_colour == CC_WHITE)
            {
                if(board[row][column] == CC_WHITE || 
                    board[row][column] == CC_K_WHITE)
                {
                    valid_move = test_for_moves(row, column, board, CC_WHITE);
                    if(valid_move)
                    {
                        /* the player can make a move */
                        return TRUE;
                    }
                }
            }
            else
            {
                if(board[row][column] == CC_RED || 
                    board[row][column] == CC_K_RED)
                {
                    valid_move = test_for_moves(row, column, board, CC_RED);
                    if(valid_move)
                    {
                        /* the player can make a move */
                        return TRUE;
                    }
                }
            }
        }
    }
    /* no piece could make a valid move */
    return FALSE;
}
/* second helper function for is_game_over(). this checks whether 
 * a piece has any valid moves */
BOOLEAN test_for_moves(int row, int column, const gameboard board, 
                                           colour player_colour)
{
    /* since there is no reference to the other player, this 
     * creates a temperary pseudo-player of the same colour to check 
     * if moves are valid */
    struct player temp_player;
    struct move move;
    BOOLEAN valid_move_range, valid_move;
    /* the distance being checked, and column and row modifiers */
    int dist, col_mod, row_mod;

    temp_player.token_colour = player_colour;

    for(dist = NORMAL_MOVE_DIST; dist <= ATTACK_MOVE_DIST; dist++)
    {
        for(row_mod = INITIAL_MOD; row_mod < END_MOD; row_mod += MOD_CHANGE)
        {
            for(col_mod = INITIAL_MOD; col_mod < END_MOD; col_mod += MOD_CHANGE)
            {
                /* creates a move, taking into account the distance, 
                 * and which diagnal via row_mod and col_mod. 
                 * create_move checks range, so an invalid move will be 
                 * rejected by the if() below */
                valid_move_range = create_move(row, column, 
                         row + dist * row_mod, column + dist * col_mod, &move);
                if(valid_move_range)
                {
                    valid_move = is_valid_move(board, &move, &temp_player);
                    if(valid_move)
                    {
                        return TRUE;
                    }
                }
            }
        }
    }
    /* there were no valid moves for this piece */
    return FALSE;
}

/**
 * This function must be called at end of each turn and the beginning of the 
 * game if you are deciding to go for the "Tracking Scores" bonus marks. 
 * This function should set the player's score to 0 and then iterate over the 
 * board and if the colour of the token in the current cell is the same as the 
 * colour of the player's token, add one to the score. 
 **/
void calc_score(gameboard board, struct player* player)
{
    int row, column;
    enum cell_contents space;
    player -> score = INITIAL_PLAYER_SCORE;

    for(row = 0; row < BOARDHEIGHT; row++)
    {
        for(column = 0; column < BOARDWIDTH; column++)
        {
            /* gets the current space */
            space = board[row][column];
            if(player -> token_colour == CC_WHITE)
            {
                /* if the player is white, they score for every 
                 * white token and white king */
                if(space == CC_WHITE || space == CC_K_WHITE)
                {
                    player -> score++;
                }
            }
            else
            {
                /* otherwise they are red, and score for every red piece */
                if(space == CC_RED || space == CC_K_RED)
                {
                    player -> score++;
                }
            }
        }
    }
}

/* basic requirements that a move must meet */
BOOLEAN do_preliminary_checks(enum cell_contents start_space, 
                              enum cell_contents end_space, 
                              struct player *player, int start_y, int end_y)
{
    /* all moves end on an empty space, so if the end isn't empty, 
     * the move is invalid */
    if(end_space != CC_EMPTY)
    {
        return FALSE;
    }

    /* the start space cannot be empty */
    if(start_space == CC_EMPTY)
    {
        return FALSE;
    }

    /* players cannot move pieces beloning to their opponent */
    if(player -> token_colour == CC_WHITE)
    {
        if(start_space == CC_RED || start_space == CC_K_RED)
        {
            return FALSE;
        }
    }
    else
    {
        if(start_space == CC_WHITE || start_space == CC_K_WHITE)
        {
            return FALSE;
        }
    }

    /* ordinary pieces can only move forward, for white 
     * that is up, so end_y should be smaller that start_x. */
    if(start_space == CC_WHITE && end_y > start_y)
    {
        return FALSE;
    }
    /* For red, start_y should be smaller */
    if(start_space == CC_RED && end_y < start_y)
    {
        return FALSE;
    }
    
    /* if none of the above tests failed, then the checks have passed */
    return TRUE;
}

/* more requirements an attack must meet */
BOOLEAN do_attack_checks(int x_distance, int y_distance, 
                        struct player *player, const gameboard board,
                        int start_x, int start_y,
                        int end_x, int end_y)
{
    int enemy_x, enemy_y;
    enum cell_contents enemy_space;

    if(x_distance == ATTACK_MOVE_DIST && y_distance == ATTACK_MOVE_DIST)
    {
        enemy_x = (start_x + end_x)/2;
        enemy_y = (start_y + end_y)/2;
        enemy_space = board[enemy_y][enemy_x];
        
        /* players can only attack enemy pieces */
        if(player -> token_colour == CC_WHITE)
        {
            if(enemy_space == CC_RED || enemy_space == CC_K_RED)
            {
                return TRUE;
            }
        }
        else
        {
            if(enemy_space == CC_WHITE || enemy_space == CC_K_WHITE)
            {
                return TRUE;
            }
        }
    }
    /* the move wasn't a valid attack */
    return FALSE;
}
