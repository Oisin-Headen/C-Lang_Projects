/***********************************************************************
 * CPT220 - Programming in C
 * Study Period 4 2016 Assignment #2 Implementation
 * Full Name        : Oisin Headen
 * Student Number   : s3513821
 * Start up code provided by Paul Miller
 ***********************************************************************/
#include "picedit_command.h"
#include "picedit_main.h"

static char *command_names[] =
    {
        "delete", "insert", "load", "print", "transfer", "write", "help",
        "exit", "quit", ""};

/*************************************************************************
 * initialize the commands array with the commands that have been defined 
 * for the system.
 ************************************************************************/
void commands_init(struct command commands[])
{
    int i;

    enum command_char command_chars[] =
        {
            CC_DELETE, CC_INSERT, CC_LOAD, CC_PRINT, CC_TRANSFER, CC_WRITE,
            CC_HELP, CC_EXIT, CC_QUIT, CC_INVALID};

    command_func funcs[] =
        {
            picedit_delete_command, picedit_insert_command, picedit_load_command,
            picedit_print_command, picedit_transfer_command, picedit_write_command,
            picedit_help_command, picedit_exit_command,
            picedit_quit_command, NULL};
    /* loops through the above arrays to init the commands array
     * this function assumes that all above arrays are in the same order*/
    for (i = 0; i < NUM_COMMANDS; i++)
    {
        strcpy(commands[i].command_name, command_names[i]);
        commands[i].command_char = command_chars[i];
        commands[i].command = funcs[i];
    }
}
/************************************************************************
 * iterates over the commands stored in the commands array and if the 
 * command_char is equal to the command char entered, we return the 
 * command object that handles the command request.
 ***********************************************************************/
struct command picedit_command_get(struct command commands[],
                                   char command_char)
{
    int i;

    for (i = 0; i < NUM_COMMANDS; i++)
    {
        if (commands[i].command_char == command_char)
        {
            return commands[i];
        }
    }
    /* the above loop will always return something, (since this is 
     * only called after processing the command), 
     * but the compiler doesn't know that.
     * returning the last option in the array to keep it happy */
    return commands[i - 1];
}

/************************************************************************
 * this command deletes a line or a range of lines from the text file. 
 * The parameters are: 
 *
 * - picedit: the datastructure that holds the current state of the 
 *   program including all the lines in the file and the file name of the
 *   file to save data back out to.
 *
 * - input: the datastructure that holds information about the line of 
 *   input read in, including the command type (delete) and the rest of 
 *   the line entered.
 ***********************************************************************/
BOOLEAN picedit_delete_command(struct picedit_system *picedit,
                               struct command_input *input)
{
    int start, end, line_num;
    /* tries to get a range from the input */
    if (!get_range(input, &start, &end))
    {
        /* this fuction can be called with a range or a single line, so 
         * tries to get a single line from the input*/
        if (!get_single_line(input, &start))
        {
            /* this command can also be used with no input, 
             * so checking for a command string */
            if (strlen(input->command_string) != 0)
            {
                printf("Error: incorect input for delete command\n");
                return FALSE;
            }
            else
            {
                if (picedit->line_list.current_line == NULL)
                {
                    printf("Error: there is no current line set\n");
                }
                /* with no input, only delete the current line */
                start = picedit->line_list.current_line->data->line_num;
                end = start;
            }
        }
        else
        {
            /* this allows the program to use the 
             * same code below for the two types of input */
            end = start;
        }
    }
    if (get_line(&(picedit->line_list), start) == NULL ||
        get_line(&(picedit->line_list), end) == NULL)
    {
        /* if one of the endpoints of the range is not valid, then 
         * we exit,since this function shouldn't do a patial delete */
        printf("Failed to delete lines %d-%d: The range specified is not "
               "valid for this file.\n",
               start, end);
        return FALSE;
    }

    for (line_num = start; line_num <= end; line_num++)
    {
        /* delete the item at the start index of the list, 
         * since the indexes will be moving down as items are deleted */
        if (list_delete(&(picedit->line_list), start) == LR_FAIL)
        {
            printf("Error deleting a line\n");
            return FALSE;
        }
    }
    /* fail message depending on what the range was */
    if (start == end)
    {
        printf("Successfully deleted line %d from the file.\n", start);
    }
    else
    {
        printf("Successfully deleted lines %d-%d from the file.\n", start, end);
    }
    return TRUE;
}

/************************************************************************
 * the command to load a new file into memory. This should behave the same
 * as specifying a file name from the command-line. Don't forget to free
 * the current datastructures before loading a new file into memory.
 ***********************************************************************/
BOOLEAN picedit_load_command(struct picedit_system *picedit,
                             struct command_input *input)
{
    /* freeing the old system, without saving it */
    picedit_free(picedit);
    /* call the utility function to load the file */
    if (!picedit_load(picedit, input->command_string))
    {
        printf("Error, could not open the file: %s\n",
               input->command_string);
        return FALSE;
    }
    return TRUE;
}

/**************************************************************************
 * this function prints out the lines requested. It should format each
 * line prior to printing so that it starts with the line number and 
 * newline characters should be inserted at the end of each line so that 
 * it nicely wraps on a 80 column terminal. If no file has yet been 
 * loaded, an appropriate error message should be displayed.
 *************************************************************************/
BOOLEAN picedit_print_command(struct picedit_system *picedit,
                              struct command_input *input)
{
    int line_num, start, end;
    struct line *line, *end_line;
    char *temp_line;
    /* try to get a range */
    if (!get_range(input, &start, &end))
    {
        /* the sample solution had the p command working with one argument, 
         * so this implementation does too */
        if (!get_single_line(input, &start))
        {
            /* this function also works with no command_string */
            if (strlen(input->command_string) != 0)
            {
                /* this will be called if the user entered input incorrectly */
                printf("Error: incorrect input for print command\n");
                return FALSE;
            }
            else
            {
                /* if the current line is NULL, start is set to one */
                if (picedit->line_list.current_line == NULL)
                {
                    start = 1;
                }
                else
                {
                    /* otherwise start is set to the line_num 
                     * of the current line*/
                    start =
                        picedit->line_list.current_line->data->line_num;
                }
                /* getting the line_num of the line 25 lines forward, 
                 * or the last line if there isn't one that far ahead */
                end_line = get_line(&(picedit->line_list), start + AFTER_CUR);
                if (end_line == NULL)
                {
                    end = picedit->line_list.last_line->data->line_num;
                }
                else
                {
                    end = end_line->line_num;
                }
            }
        }
        else
        {
            /* this is for when one argument is passed, 
             * to let the for loop iterate once */
            end = start;
        }
    }
    /* loops through the lines specified */
    for (line_num = start; line_num <= end; line_num++)
    {
        /* gets a line, then checks if it is valid */
        line = get_line(&(picedit->line_list), line_num);
        if (line == NULL)
        {
            printf("Error: %d-%d is not a valid range\n", start, end);
            return FALSE;
        }
        /* if the line length is less than 70, it does not need to be wrapped */
        if (line->length < 70)
        {
            printf(" %d: %s\n", line_num, line->line);
        }
        else
        {
            /* giving the wrap line function a copy of the line, 
             * since wrap_line modifies the string, then printing the result */
            temp_line = malloc(sizeof(char) * (line->length + NUL_TERM));
            strcpy(temp_line, line->line);
            if (!wrap_line(temp_line))
            {
                printf("Error: line had a word that was too long to print\n");
                return FALSE;
            }
            printf(" %d: %s\n", line_num, temp_line);
            free(temp_line);
        }
    }
    /* after every print command, 
     * the current line is set to the last line printed */
    set_current_line(&(picedit->line_list), end);
    return TRUE;
}

/* helper function to wrap the lines for the print command */
BOOLEAN wrap_line(char *text)
{
    int i, initial_length, steps_back;
    BOOLEAN whitespace;
    initial_length = strlen(text);
    /* loops over the string */
    for (i = 0; i <= initial_length; i++)
    {
        /* checks if the line is at it's wrapping point */
        if (i % WRAP_VALUE == 0)
        {
            /* makes sure the program doesn't try 
             * and wrap the first character */
            if (i != 0)
            {
                /* after a wrapping point has been found, 
                 * step back until a whitespace has been found */
                steps_back = 0;
                whitespace = FALSE;
                while (!whitespace)
                {
                    if (text[i] == ' ')
                    {
                        /* finds a space and changes it to the 
                         * newline, then resets i */
                        whitespace = TRUE;
                        text[i] = '\n';
                        i = i + steps_back;
                    }
                    else if (i == 0)
                    {
                        /* if i is zero, we've come all the way back 
                         * to the start of the line 
                         * so the line contains a word that is too long */
                        return FALSE;
                    }
                    else
                    {
                        /* otherwise step backwards, 
                         * keeping track of how far back i is */
                        steps_back++;
                        i--;
                    }
                }
            }
        }
    }
    return TRUE;
}

/*************************************************************************
 * This command allows the user of your program to insert some lines at 
 * the line number specified. The user types i followed by a line number.
 * As each line in entered, it is inserted into the document held in 
 * memory. Line insertions are terminated if ctrl-d or a newline character
 * are the first thing entered on a new line.
 ************************************************************************/
BOOLEAN picedit_insert_command(struct picedit_system *picedit,
                               struct command_input *input)
{
    char line[BUFSIZ + EXTRA_CHARS];
    char *err;
    int line_num;
    BOOLEAN done = FALSE;
    enum list_return lr_err;
    /* the insert command does not take a range, so getting a single line */
    if (!get_single_line(input, &line_num))
    {
        /* the insert command can take no command string */
        if (strlen(input->command_string) != 0)
        {
            printf("Error: incorrect input for insert command\n");
            return FALSE;
        }
        else
        {
            /* if possible set the insertion point to the current line */
            if (picedit->line_list.current_line == NULL)
            {
                printf("Error, the current line is not set\n");
                return FALSE;
            }
            line_num = picedit->line_list.current_line->data->line_num;
        }
    }
    while (!done)
    {
        /* using a while loop, since multiple lines can be inserted */
        printf("%d: ", line_num);
        err = fgets(line, BUFSIZ + EXTRA_CHARS, stdin);
        if (err == NULL)
        {
            /* EOF was pressed */
            done = TRUE;
            read_rest_of_line();
            printf("\n");
        }
        else
        {
            /* if fgets was successful, then add the line to the file */
            line[strlen(line) - 1] = 0;
            lr_err = list_add(&(picedit->line_list), line, line_num);
            if (lr_err == LR_MALLOC_FAILED)
            {
                picedit_free(picedit);
                exit(EXIT_FAILURE);
            }
            else if (lr_err == LR_FAIL)
            {
                printf("Error: %d is not a valid line number\n", line_num);
                return FALSE;
            }
        }
        line_num++;
    }
    return TRUE;
}

/**************************************************************************
 * this function is basically a "save as" function as it would be presented
 * in a modern text editor. You should simply save the contents of the 
 * "file" in memory to the file name provided.
 *************************************************************************/
BOOLEAN picedit_transfer_command(struct picedit_system *picedit,
                                 struct command_input *input)
{
    /* using a helper function to save to a file */
    if (!picedit_save_to_file(picedit, input->command_string))
    {
        perror("Error saving file");
        return FALSE;
    }
    return TRUE;
}

/**************************************************************************
 * saves the file currently loaded in memory back to the file it was
 * loaded from. Clearly, this should fail if there is no file loaded into
 * memory. There should be no trailing characters in the input string.
 *************************************************************************/
BOOLEAN picedit_write_command(struct picedit_system *picedit,
                              struct command_input *input)
{
    if (strlen(input->command_string) > 0)
    {
        printf("Error: the write command does not accept a command string\n");
        return FALSE;
    }
    if (!picedit_save(picedit))
    {
        perror("Error in opening file");
        return FALSE;
    }
    return TRUE;
}

/**************************************************************************
 * You should provide help text for users who are using your program. 
 * Explain how to use the different components of your program.
 *************************************************************************/
BOOLEAN picedit_help_command(struct picedit_system *picedit,
                             struct command_input *input)
{
    if (strlen(input->command_string) > 0)
    {
        printf("Error: the help command does not accept a command string\n");
        return FALSE;
    }
    /* help message had to be split accross a few #defines, 
     * since there were too many characters for the compiler */
    printf(HELP_START);
    printf(HELP_COMMANDS);
    printf(HELP_END);
    return TRUE;
}

/**************************************************************************
 * save the current file in memory and exit the program. Before you do that
 * ensure that you validate that no trailing characters were entered.
 *************************************************************************/
BOOLEAN picedit_exit_command(struct picedit_system *picedit,
                             struct command_input *input)
{
    if (strlen(input->command_string) > 0)
    {
        printf("Error: the exit command does not accept a command string\n");
        return FALSE;
    }
    /* save */
    if (!picedit_save(picedit))
    {
        perror("Error in opening file");
        return FALSE;
    }
    /* exit */
    picedit->quit = TRUE;
    return TRUE;
}

/*************************************************************************
 * simply quit the program - no save. Still you need to ensure that no 
 * trailing characters were entered.
 ************************************************************************/
BOOLEAN picedit_quit_command(struct picedit_system *picedit,
                             struct command_input *input)
{
    if (strlen(input->command_string) > 0)
    {
        printf("Error: the quit command does not accept a command string\n");
        return FALSE;
    }
    /* exit, all freeing is handled in main */
    picedit->quit = TRUE;
    return TRUE;
}

/**************************************************************************
 * receives the line of input entered by the user. Extracts the first 
 * character which is the command character. The rest of the line is the 
 * command string. Extract these values and store them in the command_input
 * pointer passed in. You should ensure that excess whitespace has been 
 * removed.
 *************************************************************************/
BOOLEAN picedit_process_command(struct line *line,
                                /* @out@ */ struct command_input *output)
{
    int cchar_counter = 0;
    BOOLEAN done = FALSE;
    char cur_char;

    if (strlen(line->line) == 0)
    {
        /* no user input */
        return FALSE;
    }
    /* gets the first non-space character, 
     * converts it to lowercase, and stores it */
    while (!done)
    {
        cur_char = line->line[cchar_counter];
        cur_char = tolower(cur_char);
        switch (cur_char)
        {
        case CC_DELETE:
        case CC_INSERT:
        case CC_LOAD:
        case CC_PRINT:
        case CC_TRANSFER:
        case CC_WRITE:
        case CC_HELP:
        case CC_EXIT:
        case CC_QUIT:
            done = TRUE;
            /* if a valid command_char was entered, 
                 * set the output cchar to it */
            output->cchar = cur_char;
            cchar_counter++;
            break;
        case ' ':
            /* ignore whitespace */
            cchar_counter++;
            break;
        default:
            done = TRUE;
            output->cchar = CC_INVALID;
            cchar_counter++;
        }
    }
    /* removing whitespace between the cchar and the command string */
    done = FALSE;
    while (!done)
    {
        cur_char = line->line[cchar_counter];
        if (cur_char == ' ')
        {
            cchar_counter++;
        }
        else
        {
            done = TRUE;
        }
    }
    /* outputing the command_string */
    output->command_string = malloc(sizeof(char) *
                                    (strlen(&(line->line[cchar_counter])) + NUL_TERM));
    if (output->command_string == NULL)
    {
        /* malloc failed, nothing can be free'd from here */
        exit(EXIT_FAILURE);
    }
    /* outputing from the start of the command_string, 
     * tracked by cchar_counter */
    strcpy(output->command_string, line->line + cchar_counter);
    return TRUE;
}

/* helper function for main, to get input and store it in a line */
enum input_result picedit_get_input(struct line *line)
{
    char input[BUFSIZ + EXTRA_CHARS];
    char *rtm_flag;

    rtm_flag = fgets(input, BUFSIZ + EXTRA_CHARS, stdin);
    /* if the user entered a massive amount of input, this will fail */
    if (input[strlen(input) - 1] != '\n')
    {
        read_rest_of_line();
        return IR_FAILURE;
    }
    input[strlen(input) - 1] = 0;

    if (rtm_flag == NULL)
    {
        return IR_RTM;
    }

    line_set_text(line, input);

    return IR_SUCCESS;
}

/* helper function to remove the whitespace from a string */
char *strip_whitespace(char *string)
{
    int i, j;
    char cur_char;
    /* includes the nul terminator, since the
     * first character has been moved to the commmand char */
    char processed_line[BUFSIZ];
    char *output;
    for (i = 0, j = 0; i < strlen(string); i++)
    {
        /* using array syntax to loop over the line */
        cur_char = string[i];
        if (cur_char != ' ')
        {
            processed_line[j] = cur_char;
            /* j is the place in the processed_line, 
             * increased when a character is added to it */
            j++;
        }
    }
    /* terminating the string */
    processed_line[j] = '\0';
    output = malloc(sizeof(char) * (strlen(processed_line) + 1));
    if (output == NULL)
    {
        /* malloc failed, nothing can be free'd from here */
        exit(EXIT_FAILURE);
    }
    strcpy(output, processed_line);
    return output;
}

/* start and end are out params. this function is used to 
 * get the range for functions that take a range */
BOOLEAN get_range(struct command_input *input, int *start, int *end)
{
    char *start_s, *end_s;
    char *end_char, *command;

    command = strip_whitespace(input->command_string);

    start_s = strtok(command, ",");
    end_s = strtok(NULL, "");
    if (start_s == NULL || end_s == NULL)
    {
        free(command);
        return FALSE;
    }
    *start = strtol(start_s, &end_char, DECIMAL);
    if (*end_char != '\0')
    {
        free(command);
        return FALSE;
    }
    *end = strtol(end_s, &end_char, DECIMAL);
    if (*end_char != '\0')
    {
        free(command);
        return FALSE;
    }
    free(command);
    return TRUE;
}

/* line_num is an out param. this function is used to get the line number
 * for functions that take a line */
BOOLEAN get_single_line(struct command_input *input, int *line_num)
{
    char *end_char, *command;

    command = strip_whitespace(input->command_string);

    if (strlen(command) == 0)
    {
        free(command);
        return FALSE;
    }
    *line_num = strtol(command, &end_char, DECIMAL);
    if (*end_char != '\0')
    {
        free(command);
        return FALSE;
    }
    free(command);
    return TRUE;
}
