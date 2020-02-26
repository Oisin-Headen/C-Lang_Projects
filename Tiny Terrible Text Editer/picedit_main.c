/***********************************************************************
 * CPT220 - Programming in C
 * Study Period 4 2016 Assignment #2 Implementation
 * Full Name        : Oisin Headen
 * Student Number   : s3513821
 * Start up code provided by Paul Miller
 ***********************************************************************/
#include "picedit_main.h"

int main(int argc, char* argv[])
{
    struct picedit_system picedit;
    struct command commands[NUM_COMMANDS];
    struct line * input_line;
    BOOLEAN loaded_successfully, input_entered, 
                    executed_successfully;

    struct command user_command;
    struct command_input processed_command;

    /* initialise the picedit structure that holds the state for 
     * the system
     */
    picedit_init(&picedit);

    /* handle command line arguments - there should either be no 
     * arguments or a single argument. If there is a single argument
     * it is a file you should open and load into the picedit_line_list
     * data structure.
     */
    if(argc == ONE_ARG)
    {
        loaded_successfully = picedit_load(&picedit, argv[FILENAME_ARG]);
        if(!loaded_successfully)
        {
            /* could not open the file for some reason */
            perror("failed to open file");
        }
    }
    else if(argc != NO_ARGS)
    {
        printf("Error: incorrect number of arguments\n");
        return EXIT_FAILURE;
    }

    /* initialise the commands structure used to handle any commands
     * entered by the user 
     */
    commands_init(commands);

    /* allocate memory for the input line */
    /* passing 0 as the line number, since no line number makes sense */
    input_line = line_init(0);
    if(input_line == NULL)
    {
        /* malloc failed, so program exits */
        return EXIT_FAILURE;
    }
    /* while the user has not decided to quit, read and process their
     * command
     */
    while(!picedit.quit)
    {
        /* prints the star at the start of each line */
        printf("* ");
    
        /* get input from the user */
        switch(picedit_get_input(input_line))
        {

            case IR_SUCCESS:
            /* process the line entered - in particular, extract the
             * command char and separate that from the rest of the 
             * command - we need to know the command char to know 
             * which command the user has requested
             */
            /* don't need to get the return value, any error will
             * turn the processed_command's cchar to CC_INVALID */
            input_entered = picedit_process_command(input_line, 
                                                &processed_command);

            if(!input_entered)
            {
                /* no user input, so just skip back to the start of the loop */
                continue;
            }

            /* get the function pointer for the command that has 
             * been entered
             */
            user_command = picedit_command_get(commands, 
                                                   processed_command.cchar);  
            /* if what was entered was an invalid command, display
             * an error message 
             */
            if(user_command.command_char == EOF)
            {
                /* if the command was invalid, the text entered
                 * by the user is stored in the command_string */
                printf("Error: %s is not a valid command\n", 
                                        processed_command.command_string);
            }
            else
            {

                /* otherwise perform the command */
                executed_successfully = user_command.command(&picedit, 
                                                           &processed_command);
                if(!executed_successfully)
                {
                    /* if the command failed, display an 
                     * error message which shows which 
                     * action was attempted and a failure 
                     * message
                     */
                    printf("Error: the %s command failed\n", 
                                                user_command.command_name);
                }
            }
            /* frees the malloc'ed section of the command_input */
            free(processed_command.command_string);
            break;

        case IR_FAILURE:
            printf("Error, too much input entered\n");
            break;

        case IR_RTM:
            /* nothing happens here */
            break;
        }
        free_line(input_line);
    }
    /* don't forget to free memory before exiting */
    picedit_free(&picedit);
    free(input_line);
    free((char *)picedit.current_file);
    return EXIT_SUCCESS;
}

