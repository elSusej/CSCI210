#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW
    
    if (strlen(pathName) == 0 || strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64]; // Since we can't make string variables.
    char dirName[256]; 
    
    char pathCopy[256];
    strncpy(pathCopy, pathName, sizeof(pathCopy) - 1);
    pathCopy[sizeof(pathCopy) - 1] = '\0';
    
    struct NODE* parentDir = splitPath(pathCopy, baseName, dirName);

    // I kept on getting a segfault when the parent directory did not exist without this, which makes sense since we can't make a folder in a non-existent directory.
    if (parentDir == NULL) {
        return;
    }

    // Check if directory already exists in parent
    struct NODE* current = parentDir->childPtr;
    while (current != NULL) {
        if (strcmp(current->name, baseName) == 0) {
            if (current->fileType == 'D') {
                printf("MKDIR ERROR: directory %s already exists\n", pathName);
                return;
            }
        }
        current = current->siblingPtr;
    }

    // Create new directory node.
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strncpy(newDir->name, baseName, sizeof(newDir->name) - 1);
    newDir->name[sizeof(newDir->name) - 1] = '\0';
    newDir->fileType = 'D';
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;
    newDir->parentPtr = parentDir;

    // If parent has no children, make this the first child
    if (parentDir->childPtr == NULL) {
        parentDir->childPtr = newDir;
    } else {
        // Otherwise, add as last sibling
        current = parentDir->childPtr;
        while (current->siblingPtr != NULL) {
            current = current->siblingPtr;
        }
        current->siblingPtr = newDir;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", newDir->name);
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
