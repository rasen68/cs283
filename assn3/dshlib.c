#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"
#define SPACE_STRING " "

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
	while (*cmd_line == SPACE_CHAR) { cmd_line++; }
	if (*cmd_line == PIPE_CHAR || *cmd_line == '\0') {
		return WARN_NO_CMDS; 
	}

	char* buffer = strtok(cmd_line, PIPE_STRING);
	while (buffer != NULL) {
		while (*buffer == SPACE_CHAR) { buffer++; }

		clist->num += 1;
		if (clist->num > CMD_MAX) { return ERR_TOO_MANY_COMMANDS; }

		command_t* currentCommand = &(clist->commands[clist->num - 1]);
		char* args = strchr(buffer, SPACE_CHAR);
		if (args == NULL) { args = strchr(buffer, '\0'); }
	
		if (args - buffer > EXE_MAX) {
			return ERR_CMD_OR_ARGS_TOO_BIG;
		}
		if (args != NULL && strlen(args) >= ARG_MAX) {
			return ERR_CMD_OR_ARGS_TOO_BIG;
		}

		char* end = args + strlen(args) - 1;
		while (*end == SPACE_CHAR) { end--; } end++;
		*end = '\0';
		strncpy(currentCommand->exe, buffer, args - buffer);
		while (*args == SPACE_CHAR) { args++; }
		strcpy(currentCommand->args, args);

		buffer = strtok(NULL, PIPE_STRING);
	}
	return OK;
}
