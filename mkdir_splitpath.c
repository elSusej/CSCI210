#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    if (strlen(pathName) == 0 || strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64];
    char dirName[256];
    char pathCopy[256];

    strncpy(pathCopy,pathName, sizeof(pathCopy) -1);
    pathCopy[sizeof(pathCopy) - 1] = '\0';

    struct NODE* parent = splitPath(pathCopy, baseName, dirName);
    
    if (parent == NULL) {
        return;
    }

    //check if directory already exists:
    struct NODE* currentDir = parent->childPtr;
    while (currentDir != NULL) {
        if ((strcmp(currentDir->name,baseName) == 0) && (currentDir->fileType == 'D')) {
            printf("MKDIR ERROR: directory %s already exists\n", currentDir->name);
            return;
        }

        currentDir = currentDir->siblingPtr;
    }

    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strncpy(newDir->name, baseName, sizeof(newDir->name) - 1);
    newDir->name[sizeof(newDir->name) - 1] = '\0';
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parent;

    if (parent->childPtr == NULL) {
        parent->childPtr = newDir;
    }
    else {
        currentDir = parent->childPtr;
        while (currentDir->siblingPtr != NULL) {
            currentDir = currentDir->siblingPtr;
        }
        currentDir->siblingPtr = newDir;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", newDir->name);
    return; //
}

//handles tokenizing and absolute/relative pathing options

struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    // "The function splitPath() with the following signature gets the complete path string to a file or directory as a relative or an absolute path" Checks for that by indexing first char.
    struct NODE* currentNode;
    if (pathName[0] == '/') {
        currentNode = root;  
    } else {
        currentNode = cwd;   
    }

    // Handle special case for root "/" Lowk hacky lmaoooooo, because when you tokenize a single / with nothing, tokenizedPathName is null, and the while loop below doesn't hit work:
    // and we end up with nothing in baseName and dirName.
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    // Make a copy of pathName for tokenization
    char pathCopy[256];
    strncpy(pathCopy, pathName, sizeof(pathCopy) - 1);
    pathCopy[sizeof(pathCopy) - 1] = '\0';

    char* tokenizedPathName = strtok(pathCopy, "/");
    strcpy(dirName, "");

    char* lastToken = NULL;
    char tempPath[256] = "";

    while (tokenizedPathName != NULL) {
        if (lastToken != NULL) {
            if (strlen(tempPath) > 0) {
                strcat(tempPath, "/");
            }
            strcat(tempPath, lastToken);
        }
        lastToken = tokenizedPathName;
        tokenizedPathName = strtok(NULL, "/");
    }

    if (lastToken != NULL) {
        strcpy(baseName, lastToken);
    } else {
        strcpy(baseName, "");
    }

    if (strlen(tempPath) > 0) {
        if (pathName[0] == '/') {
            strcpy(dirName, "/");
            strcat(dirName, tempPath);
        } else {
            strcpy(dirName, tempPath);
        }
    } else if (pathName[0] == '/') {
        strcpy(dirName, "/");
    }

    if (strlen(dirName) > 0 && strcmp(dirName, "/") != 0) {
        char dirCopy[256];
        strncpy(dirCopy, dirName, sizeof(dirCopy));
        char* dirToken = strtok(dirCopy, "/");
        
        while (dirToken != NULL) {
            struct NODE* child = currentNode->childPtr;
            int found = 0;

            while (child != NULL) {
                if (strcmp(child->name, dirToken) == 0 && child->fileType == 'D') {
                    currentNode = child;
                    found = 1;
                    break;
                }
                child = child->siblingPtr;
            }

            if (!found) {
                printf("ERROR: directory %s does not exist\n", dirToken);
                return NULL;
            }

            dirToken = strtok(NULL, "/");
        }
    }

    return currentNode;
}
