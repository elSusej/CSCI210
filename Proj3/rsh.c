#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char* cmd) {
    // TODO
    // return 1 if cmd is one of the allowed commands
    // return 0 otherwise
    for (int i = 0; i < 12; i++) {
        if ( strcmp(cmd,allowed[i]) == 0 ) {
            return 1;
        }
    }
    
    return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
    char linecpy[256];
    char* argv[21];
    char* token;
	pid_t pid;
    int status;

    while (1) {

    fprintf(stderr,"rsh>");

    if (fgets(line,256,stdin)==NULL) continue;

    if (strcmp(line,"\n")==0) continue;

    line[strlen(line)-1]='\0';

    strcpy(linecpy, line);
    token = strtok(linecpy, " ");
    int i = 0;

    while (token != NULL) {
        argv[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    argv[i] = NULL;

    if (isAllowed(argv[0]) == 0) {
        printf("NOT ALLOWED!\n");
        continue;
    }
    else if (strcmp(argv[0],"exit") == 0) {
        break;
    }
    else if (strcmp(argv[0],"help") == 0) {
        printf("The allowed commands are:\n");
        for (int i = 1; i <= 12; i++) {
            printf("%d: %s\n", i, allowed[i-1]);
        }
        continue;
    }
    else if (strcmp(argv[0],"cd") == 0) { //argv[1] != NULL) && (argv[2] != NULL)
        if (i > 2) { //more than 1 argument
            printf("-rsh: cd: too many arguments");
        }
        else if (i == 1){ //no argument
            chdir(getenv("HOME"));
        }
        else{ //exactly 1 argument
            chdir(argv[1]);
        }
        continue;
    }

    status = posix_spawnp(&pid, argv[0], NULL, NULL, argv, environ);

    if (status == 0) {
        if (waitpid(pid, &status, 0) == -1) {
            continue;
        }
    }

    }
    return 0;
}