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
    char* argv[20];
    char* token;
	pid_t pid;
    int status;
    posix_spawnattr_t attr;
	posix_spawnattr_init(&attr);

    while (1) {

    fprintf(stdout,"rsh>");

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
    }
    // TODO
    // Add code to spawn processes for the first 9 commands
    // And add code to execute cd, exit, help command
    
    if (strcmp(argv[0],"cp") == 0) {
        if (posix_spawnp(&pid, "cp", NULL, &attr, argv, environ) != 0) {
            perror("spawn failed");
            exit(EXIT_FAILURE);
        }

        // Wait for the spawned process to terminate
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            printf("Spawned process exited with status %d\n", WEXITSTATUS(status));
        }

        // Destroy spawn attributes
        posix_spawnattr_destroy(&attr);
    }
    if (strcmp(argv[0],"touch") == 0) {
        if (posix_spawnp(&pid, "touch", NULL, &attr, argv, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    posix_spawnattr_destroy(&attr);
        
    }
    if (strcmp(argv[0],"mkdir") == 0) {
        if (posix_spawnp(&pid, "mkdir", NULL, &attr, argv, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    posix_spawnattr_destroy(&attr);
    }

    if (strcmp(argv[0],"ls") == 0) {
        if (posix_spawnp(&pid, "ls", NULL, &attr, argv, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    posix_spawnattr_destroy(&attr);
    }

    if (strcmp(argv[0],"pwd") == 0) {
        if (posix_spawnp(&pid, "pwd", NULL, &attr, argv, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    posix_spawnattr_destroy(&attr);
    }
    if (strcmp(argv[0],"cat") == 0) {
        if (posix_spawnp(&pid, "cat", NULL, &attr, argv, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    posix_spawnattr_destroy(&attr);
    }
    if (strcmp(argv[0],"grep") == 0) {
        if (posix_spawnp(&pid, "grep", NULL, &attr, argv, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    posix_spawnattr_destroy(&attr);
        
    }
    if (strcmp(argv[0],"chmod") == 0) {
        if (posix_spawnp(&pid, "chmod", NULL, &attr, argv, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    posix_spawnattr_destroy(&attr);
        
    }
    if (strcmp(argv[0],"diff") == 0) {
        if (posix_spawnp(&pid, "diff", NULL, &attr, argv, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
    }

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    posix_spawnattr_destroy(&attr);
    }
    
    if (strcmp(argv[0],"cd") == 0) {
        if ((argv[1] != NULL) && (argv[2] != NULL)) {
            printf("-rsh: cd: too many arguments");
        }
        else {
            chdir(argv[1]);
        }
    }
    if (strcmp(argv[0],"exit") == 0) {
        break;
    }
    if (strcmp(argv[0],"help") == 0) {
        printf("The allowed commands are:\n");
        for (int i = 1; i <= 12; i++) {
            printf("%d: %s\n", i, allowed[i-1]);
        }
    }
    // Use the example provided in myspawn.c

    }
    return 0;
}