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
			return BI_NOT_BI;
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

int execute_pipeline(command_list_t *clist) {
	int pipes[clist->num - 1][2];
	pid_t pids[clist->num];
	int pids_st[clist->num];
	int exit_code;

	for (int i = 0; i < clist->num - 1; i++) {
		if (pipe(pipes[i]) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < clist->num; i++) {
		pids[i] = fork();
		if (pids[i] == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if (pids[i] == 0) {
			if (i > 0) {
				dup2(pipes[i-1][0], STDIN_FILENO);
			}

			if (i < clist->num - 1) {
				dup2(pipes[i][1], STDOUT_FILENO);
				dup2(pipes[i][1], STDERR_FILENO);
			}

			for (int j = 0; j < clist->num - 1; j++) {
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			clist->commands[i].argv[clist->commands[i].argc] = NULL;

			execvp(clist->commands[i].argv[0], clist->commands[i].argv);
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < clist->num - 1; i++) {
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	for (int i = 0; i < clist->num; i++) {
		waitpid(pids[i], &pids_st[i], 0);
	}

	exit_code = WEXITSTATUS(pids_st[clist->num - 1]);
	for (int i = 0; i < clist->num; i++) {
		if (WEXITSTATUS(pids_st[i]) == EXIT_SC) 
			exit_code = EXIT_SC;
	}
	return exit_code;
}
	
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
	 while (*cmd_line == SPACE_CHAR) { cmd_line++; }
	 if (cmd_line == NULL || *cmd_line == '\0' || *cmd_line == PIPE_CHAR) {
		 return WARN_NO_CMDS;
	 }

	 cmd_buff->argc = 1;

	 cmd_buff->argv[0] = malloc(CMD_ARGV_MAX * sizeof(char));
	 char* end_of_exe = strchr(cmd_line, SPACE_CHAR);
	 if (end_of_exe != NULL) {
		 int end_index = end_of_exe - cmd_line;
		 if (end_index > EXE_MAX) {
			 return ERR_CMD_OR_ARGS_TOO_BIG;
		 }
	 	 strncpy(cmd_buff->argv[0], cmd_line, end_index); 
		 cmd_buff->argv[0][end_index] = '\0';
	 } else {
		 if (strlen(cmd_line) > EXE_MAX) {
			 return ERR_CMD_OR_ARGS_TOO_BIG;
		 }
		 strcpy(cmd_buff->argv[0], cmd_line);
	 }

	 char* buffer = strchr(cmd_line, SPACE_CHAR);
	 while (buffer != NULL) {
	 	while (*buffer == SPACE_CHAR) { buffer++; }
		if (*buffer == '\0') { break; }

		if (cmd_buff->argc > CMD_ARGV_MAX) {
			return ERR_CMD_OR_ARGS_TOO_BIG;
		}

	 	cmd_buff->argv[cmd_buff->argc] = malloc(CMD_ARGV_MAX * sizeof(char));
	   if (buffer[0] == '"') {
			char* end_of_quote = strchr(buffer+1, '"');
			int quote_index = end_of_quote - (buffer+1);
			strncpy(cmd_buff->argv[cmd_buff->argc], buffer+1, quote_index);
		 	cmd_buff->argv[cmd_buff->argc][quote_index] = '\0';

			buffer += quote_index+1;

		} else {

	 		char* end_of_arg = strchr(buffer, SPACE_CHAR);
			int arg_index = end_of_arg - buffer;
	 		if (end_of_arg != NULL) {
	 	 		strncpy(cmd_buff->argv[cmd_buff->argc], buffer, arg_index); 
				cmd_buff->argv[cmd_buff->argc][arg_index] = '\0';

	 		} else {
		 		strcpy(cmd_buff->argv[cmd_buff->argc], buffer);
			}

		}	
	 	buffer = strchr(buffer, SPACE_CHAR);
	   cmd_buff->argc += 1;
	 }
	 return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
	clist->num = 0;
	while (*cmd_line == SPACE_CHAR) { cmd_line++; }
	if (*cmd_line == PIPE_CHAR || *cmd_line == '\0') {
		return WARN_NO_CMDS;
	}

	char* buffer = strtok(cmd_line, PIPE_STRING);
	while (buffer != NULL) {
		while (*buffer == SPACE_CHAR) { buffer++; }

		clist->num += 1;
		if (clist->num > CMD_MAX) { return ERR_TOO_MANY_COMMANDS;}
		cmd_buff_t* currentCommand = &(clist->commands[clist->num - 1]);

		int rc = build_cmd_buff(buffer, currentCommand);
		if (rc == WARN_NO_CMDS) {
			return WARN_NO_CMDS; 
		} else if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {
			return ERR_CMD_OR_ARGS_TOO_BIG;
		}

		buffer = strtok(NULL, PIPE_STRING);
	}
	return OK;
}

int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));

    while(1){
	 	 printf("%s", SH_PROMPT);
       if (fgets(cmd_buff, ARG_MAX, stdin) == NULL) {
          printf("\n");
          break;
       }

       //remove the trailing \n from cmd_buff
       cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

		 command_list_t* commandList = malloc(sizeof(command_list_t));
		 int rc = build_cmd_list(cmd_buff, commandList);

		 if (rc == WARN_NO_CMDS) { printf(CMD_WARN_NO_CMD); }
		 else if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {
			 printf(CMD_ERR_TOO_BIG);
		 } else if (rc == OK) {
			 if (match_command(commandList->commands[0].argv[0]) != BI_NOT_BI) {
				 int rc = exec_built_in_cmd(&(commandList->commands[0]));
				 if (rc == BI_NOT_BI) { printf(ERR_EXEC); }
				 else if (rc == BI_CMD_EXIT) { return OK_EXIT; }
				 else if (rc != BI_EXECUTED) { 
					 printf(ERR_UNKNOWN);
					 return ERR_EXEC_CMD;
				 }
			 } else {
				 pid_t supervisor = fork();
				 if (supervisor == -1) {
					 perror("fork supervisor");
					 exit(EXIT_FAILURE);
				 }
				 if (supervisor == 0) {
					 execute_pipeline(commandList);
					 exit(EXIT_SUCCESS);
				 }

				 waitpid(supervisor, NULL, 0);
			 }
		 } else { printf(ERR_UNKNOWN); }
		 for (int i = 0; i < commandList->num; i++) {
			 for (int j = 0; j < commandList->commands[i].argc; j++) {
				 free(commandList->commands[i].argv[j]);
			 }
		 }
		 free(commandList);
     }

	 free(cmd_buff);
    return OK;
}
