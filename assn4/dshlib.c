#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

#define SPACE_STRING " "
#define DRAGON_CMD "dragon"
#define CD_CMD "cd"
#define CMD_ERR_TOO_BIG "error: command too long\n"
#define ERR_EXEC "error: couldnt execute command\n"
#define M_NOT_IMPL "not implemented!\n"
#define ERR_UNKNOWN "unknown error\n"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
	for (int i = 0; i < CMD_ARGV_MAX; i++) {
		cmd_buff->argv[i] = malloc(sizeof(char) * ARG_MAX);
		if (cmd_buff->argv[i] == NULL) {
			return(ERR_MEMORY);
		}
	}
	cmd_buff->argc = 1;
	return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
	free_cmd_buff(cmd_buff);
	return alloc_cmd_buff(cmd_buff);
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
	for (int i = 0; i < CMD_ARGV_MAX; i++) {
		free(cmd_buff->argv[i]);
	}
	return OK;
}

Built_In_Cmds match_command(const char *input) {
	if (strcmp(input, EXIT_CMD) == 0) {
		return BI_CMD_EXIT; 
	}

	if (strcmp(input, DRAGON_CMD) == 0) {
		return BI_CMD_DRAGON;
	}

	if (strcmp(input, CD_CMD) == 0) {
		return BI_CMD_CD;
	}

	return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
	if (strcmp(cmd->argv[0], CD_CMD) == 0) {
		if (cmd->argc > 1) {
			int rc = chdir(cmd->argv[1]);
			if (rc == 0) { return BI_EXECUTED; }
			return BI_RC;
		}
		return BI_EXECUTED;
	}
	if (strcmp(cmd->argv[0], DRAGON_CMD) == 0) {
		printf(M_NOT_IMPL);
		return BI_EXECUTED;
	}
	if (strcmp(cmd->argv[0], EXIT_CMD) == 0) {
		return BI_CMD_EXIT;
	}
	return BI_NOT_BI;
}

int exec_cmd(cmd_buff_t *cmd) {
	if (match_command(cmd->argv[0]) != BI_NOT_BI) {
		int rc = exec_built_in_cmd(cmd);
		if (rc == BI_RC || rc == BI_NOT_BI) { printf(ERR_EXEC); }
		else if (rc == BI_CMD_EXIT) { return BI_CMD_EXIT; }
		else if (rc == BI_EXECUTED) { return OK; }
		else { printf(ERR_UNKNOWN); }

		return ERR_EXEC_CMD;
	} else {
		int f_result, c_result;

		f_result = fork();
		if (f_result < 0) {
			printf(ERR_EXEC);
			return ERR_EXEC_CMD;
		}

		if (f_result == 0) {
			char* arglist[cmd->argc+1];
			for (int i = 0; i < cmd->argc; i++) {
				arglist[i] = malloc(strlen(cmd->argv[i])+1);
				strcpy(arglist[i], cmd->argv[i]);
			}
			arglist[cmd->argc] = NULL;

			int rc = execvp(arglist[0], arglist);
			if (rc < 0) {
				printf(ERR_EXEC);
				exit(42);
			}
		} else {
			wait(&c_result);
			if (WEXITSTATUS(c_result) != 0) {
				return ERR_EXEC_CMD;
			}
			return OK;
		}
	}
}
	
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
	 while (*cmd_line== SPACE_CHAR) { cmd_line++; }
	 if (cmd_line == NULL || *cmd_line == '\0') {
		 return WARN_NO_CMDS;
	 }

	 char* tempbuffer = malloc(sizeof(char) * SH_CMD_MAX);
	 strcpy(tempbuffer, cmd_line);
	 char* endbuffer = strchr(tempbuffer, SPACE_CHAR);
	 if (endbuffer != NULL) { *endbuffer = '\0'; }
	 if (strlen(tempbuffer) > EXE_MAX) { return ERR_CMD_OR_ARGS_TOO_BIG; }
	 strcpy(cmd_buff->argv[0], tempbuffer);

	 char* buffer = strchr(cmd_line, SPACE_CHAR);
	 while (buffer != NULL) {
	 	while (*buffer == SPACE_CHAR) { buffer++; }

		if (cmd_buff->argc > CMD_ARGV_MAX) {
			return ERR_CMD_OR_ARGS_TOO_BIG;
		}

	   if (buffer[0] == '"') {
			char *quotestring = malloc(sizeof(char) * (strlen(buffer) - 1));

			int i = 1;
			while (buffer[i] != '"') {
				quotestring[i-1] = buffer[i];
				if (buffer[i] == ' ') {
					buffer[i] = '0';
				}
				i++;
			}

			quotestring[i] = '\0';

			strcpy(cmd_buff->argv[cmd_buff->argc], quotestring);
		} else {
			strcpy(cmd_buff->argv[cmd_buff->argc], buffer);
	 		strcpy(tempbuffer, buffer);
	 		char* endbuffer = strchr(tempbuffer, SPACE_CHAR);
	 		if (endbuffer != NULL) { *endbuffer = '\0'; }

	 		strcpy(cmd_buff->argv[cmd_buff->argc], tempbuffer);
		}
	 	buffer = strchr(buffer, SPACE_CHAR);
	   cmd_buff->argc += 1;
	 }
	 return OK;
}

int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    cmd_buff_t *cmd = malloc(sizeof(cmd_buff_t));
	 if (alloc_cmd_buff(cmd) != OK) { return ERR_MEMORY; }

    // TODO IMPLEMENT MAIN LOOP
    while(1){
	 	 printf("%s", SH_PROMPT);
       if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
          printf("\n");
          break;
       }
       //remove the trailing \n from cmd_buff
       cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

       //IMPLEMENT THE REST OF THE REQUIREMENTS
		 if (clear_cmd_buff(cmd) != OK) { return ERR_MEMORY; }
		 int rc = build_cmd_buff(cmd_buff, cmd);

		 if (rc == WARN_NO_CMDS) { printf(CMD_WARN_NO_CMD); }
		 else if (rc == OK) {
			 if (exec_cmd(cmd) == BI_CMD_EXIT) {
				 return OK_EXIT;
			 }
		 } else if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {
			 printf(CMD_ERR_TOO_BIG);
		 } else { printf(ERR_UNKNOWN); }
     }

	 free(cmd);
    return OK;
}
