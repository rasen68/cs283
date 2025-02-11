#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
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
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */
int main()
{
    char *cmd_buff = malloc(SH_CMD_MAX);

	 while (1) {
		 printf("%s", SH_PROMPT);
		 if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
			 printf("\n");
			 break;
		 }

		 cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

	 	 command_list_t* commandList = malloc(sizeof(command_list_t));
		 int rc = build_cmd_list(cmd_buff, commandList);

		 if (rc == -1) { printf(CMD_WARN_NO_CMD); continue; }
		 if (rc == -2) { printf(CMD_ERR_PIPE_LIMIT, CMD_MAX); continue; }
		 if (rc == -3) { printf(M_NOT_IMPL); continue; }
		 
		 if (strcmp(commandList->commands[0].exe, EXIT_CMD) == 0) {
			 return OK;
		 }

		 printf(CMD_OK_HEADER, commandList->num); 
		 for (int i = 0; i < commandList->num; i++) {
			 printf("<%d> %s", i+1, commandList->commands[i].exe);
			 if (strlen(commandList->commands[i].args) > 0) {
				 printf(" [%s]", commandList->commands[i].args);
			 }
			 printf("\n");
		 }
	 }
}
