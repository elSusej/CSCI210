// Resources used: Stackoverflow (Linked), 
// week 10 lecture posix, waitpid, perror, week 8 for strings. 
// https://cplusplus.com/reference/ for general (I know its C++ but it works for C i found out)


#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	
	// https://stackoverflow.com/questions/8004237/how-do-i-properly-compare-strings-in-c
	// https://stackoverflow.com/questions/1597830/iterate-through-a-c-array

    for (size_t i = 0; i < sizeof(allowed) / sizeof(allowed[0]); i++) {
        // Check if cmd matches any allowed command
        if (strcmp(allowed[i], cmd) == 0) {
            return 1;  // Command is allowed
        }
    }
    return 0;  // Command is not allowed

}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
	char *args[21];  // max 20 arguments + NULL terminator
    char line_copy[256];
    int status;
    pid_t pid;


    while (1) {

	// No indents?????? Yall want to see the world burn.
	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c

		strcpy(line_copy, line);  // Make a copy for second tokenization if needed
        
        // Tokenize input
        char *token = strtok(line, " ");
        int arg_count = 0;
        
        // Get command and arguments
        while (token != NULL && arg_count < 20) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
		
        args[arg_count] = NULL;  // NULL terminate the argument array
        
        // Check if command is allowed
        if (!isAllowed(args[0])) {
            printf("NOT ALLOWED!\n");
            continue;
        }
        
        // Handle built-in commands
        if (strcmp(args[0], "exit") == 0) {
            return 0;
        }
        else if (strcmp(args[0], "help") == 0) {
            printf("The allowed commands are:\n");
            printf("1: cp\n");
            printf("2: touch\n");
            printf("3: mkdir\n");
            printf("4: ls\n");
            printf("5: pwd\n");
            printf("6: cat\n");
            printf("7: grep\n");
            printf("8: chmod\n");
            printf("9: diff\n");
            printf("10: cd\n");
            printf("11: exit\n");
            printf("12: help\n");
            continue;
        }
        else if (strcmp(args[0], "cd") == 0) {
            if (arg_count > 2) {  // More than one argument for cd
                printf("-rsh: cd: too many arguments\n"); 
            }
            else if (arg_count == 1) {  // No argument, go to home directory
                chdir(getenv("HOME"));
            }
            else {  // One argument
                if (chdir(args[1]) != 0) {
                    perror("cd");
                }
            }
            continue;
        }
        
        // Spawn process for allowed external commands
        status = posix_spawnp(&pid, args[0], NULL, NULL, args, environ); // (Similar to `posix_spawn' but search for FILE in the PATH.)
        
        if (status == 0) {
            if (waitpid(pid, &status, 0) != -1) { // Wait for spawned process to finish. (Ed-discussion common errors) Returns -1 if unsucc.
                continue;
            }
        }
    }

    return 0;

}