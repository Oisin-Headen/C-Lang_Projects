/***********************************************************************
* CPT220 - Programming in C
* Study Period 4 2016 Assignment #2 Implementation
* Full Name        : Oisin Headen
* Student Number   : s3513821
* Start up code provided by Paul Miller
***********************************************************************/
#include "picedit_shared.h"
#include "picedit_utility.h"
#include "picedit_line_list.h"
#include <stdlib.h>
#include <string.h>
#ifndef PICEDIT_COMMAND_H
#define PICEDIT_COMMAND_H

/* how many commands are available in the system ? This includes a "dummy"
 * command at the end of the array
 */
#define NUM_COMMANDS 10

/*the maximum length of each command name */
#define COMMAND_NAME_LEN 10

#define NUL_TERM 1
#define DECIMAL 10
#define WRAP_VALUE 70
/* 24 plus the current line is the 25 lines needed */
#define AFTER_CUR 24

/* the help strings have to be split up, since there are too many chars */
#define HELP_START "Welcome to PICedit (Programing in C Edit)\n"\
             "-----------------------------------------\n"\
             "The following commands are valid in picedit:\n\n"
             
#define HELP_COMMANDS "l followed by a filename:\n\tload a file into memory\n"\
             "i followed by a line_number: start inserting text at a line\n"\
             "\tThe line must be within the range of the document\n"\
             "d followed by either a line number or a range:\n"\
             "\tdelete the range or line number given\n"\
             "p followed by either a line number or a range:\n"\
             "\tprint out the specified lines\n"\
             "t followed by a filename:\n"\
             "\tsave the current file to the file specified\n"\
             "w:\n"\
             "\tsave the file\n"\
             "e:\n"\
             "\tsave the file and exit\n"\
             "q:\n"\
             "\texit the program without saving\n\n"

#define HELP_END "I attempted the bonus marks requirements, so p, i, \n"\
                 "and d can be called without a line number or range,\n"\
                 "instead preforming the commands from the current line.\n"\
                 "Those commands all affect the placement of the line,\n"\
                 "The last line printed and the last line inserted are\n"\
                 "always set to the current line, and if the current line is\n"\
                 "deleted, then the current line is set to the next line\n\n"\
                 "All commands can be entered in upper or lower case,\n"\
                 "and all whitespace is ignored.\n"



/* forwards declaration of the picedit_system struct. We don't need to 
 * know the details in this header file but several of the functions 
 * in this module use pointers to it so the compiler does need to know there
 * will be a datatype with that name.
 */
struct picedit_system;

/**
 * The list of valid command chars that can be entered at a normal prompt
 * with this program. Please note that you must be
 * able to handle these in upper or lower case. Note that
 * CC_INVALID simply marks then end of the enumeration, and an
 * invalid command_char.
 **/
enum command_char
{
        CC_DELETE='d', CC_PRINT='p', CC_INSERT='i',
        CC_TRANSFER='t', CC_WRITE='w', CC_EXIT='e', CC_QUIT='q',
        CC_LOAD = 'l', CC_HELP='h', CC_INVALID=EOF
};

/**
 * This structure is provided so you can handle user input and then
 * pass it to the various commands handled by the system
 **/
struct command_input
{
        /* which character was selected for this command ? */
        enum command_char cchar;
        /* the rest of the command string entered after removing 
         * the command char
         */
        char* command_string;
};

/**
 * typedef of the function pointer type for all functions that implement
 * a command in the system. we create an alias for the function pointers
 * so they are easier to manage in our code. In fact without this typedef
 * the declarations got so complicated that they confused the compiler. 
 * We now have a type called command_func that is a pointer to a function.
 **/
typedef BOOLEAN (*command_func)(struct picedit_system*, 
                struct command_input*);

/**
 * The datastructure that represents a command - the command is the struct
 * that carries information about the different command requests possible
 * in the system.
 **/
struct command
{
        /* the char that calls this command */
        char command_char;
        /* a pointer to the function to be called */
        command_func command;
        /* the name by which the function is referred to, eg: print, delete,
         * transfer, etc.
         */
        char command_name[COMMAND_NAME_LEN + 1];
};

void commands_init(struct command[]);

struct command picedit_command_get(struct command[], char);

/* functions that implement the various commands in the system */
BOOLEAN picedit_delete_command(struct picedit_system*,  
                struct command_input*);

BOOLEAN picedit_print_command(struct picedit_system*, 
                struct command_input*);
/* helper for the print command */
BOOLEAN wrap_line(char *);

BOOLEAN picedit_insert_command(struct picedit_system*, 
                struct command_input*);

BOOLEAN picedit_load_command(struct picedit_system*, struct command_input*);

BOOLEAN picedit_transfer_command(struct picedit_system*, 
                struct command_input*);

BOOLEAN picedit_write_command(struct picedit_system*, 
                struct command_input*);

BOOLEAN picedit_exit_command(struct picedit_system*, struct command_input*);

BOOLEAN picedit_quit_command(struct picedit_system*, struct command_input*);

BOOLEAN picedit_help_command(struct picedit_system*, struct command_input*);

/* process command input entered at the picedit prompt to prepare 
 * for calling the command function
 */
BOOLEAN picedit_process_command(struct line *, struct command_input*);

/* read input from the keyboard */
enum input_result picedit_get_input(struct line *);
char* strip_whitespace(char*);
BOOLEAN get_range(struct command_input*, int*, int*);
BOOLEAN get_single_line(struct command_input*, int*);
#endif
