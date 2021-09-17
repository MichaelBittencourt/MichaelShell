#include"mshell.h"
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <limits.h>

#define QTD_COMMANDS 2
#define GET_CURRENT_PATH_ERROR 2
#define ERROR_WHEN_READ_INPUT 3

typedef struct MShell {
    char * internCommands[QTD_COMMANDS];
    char * path;
    char pwd[PATH_MAX+1];
    char * prompt;
    int lastReturn;
    boolean parallel;
} MShell;

char * createString(const char * text) {
    char *  ret = (char *)malloc(sizeof(char)*strlen(text));
    strcpy(ret, text);
    return ret;
}

MShell createMShell() {
    MShell msh;
    msh.internCommands[1] = createString("cd");
    msh.internCommands[2] = createString("pwd");
    msh.path = createString("/bin/");
    if (getcwd(msh.pwd, sizeof(msh.pwd)) == NULL) {
        exit(GET_CURRENT_PATH_ERROR);
    }
    msh.prompt = createString("$> ");
    msh.lastReturn = 0;
    return msh;
}

void deleteMShell(MShell * msh) {
    for ( int i = 0; i < QTD_COMMANDS; i++) {
        free(msh->internCommands[i]);
    }
    free(msh->path);
}

void printMShell(MShell *msh) {
    printf("MShell settings:\n");
    printf("\tAll intern commands:\n");
    for(int i = 0; i < QTD_COMMANDS; i++) {
        printf("\t    command[%d]: '%s'\n", i, msh->internCommands[i]);
    }
    printf("\tPath: '%s':\n", msh->path);
    printf("\tCurrent Directory: '%s'\n", msh->pwd);
    printf("\tPrompt: '%s'\n", msh->prompt);
}

boolean runIntern(MShell * msh, const char *input) {
    printf("\nRunning Intert command: %s", input);
}

boolean runBinary(MShell * msh, const char *input) {
    printf("\nRunning binary: %s", input);
}

boolean runCommand(MShell * msh, const char *input) {
    boolean ret = runIntern(msh, input);
    if (ret) {
        ret = runBinary(msh, input);
    }
    return ret;
}

void printPrompt(MShell * msh) {
    printf("\n%s", msh->prompt);
}

int runMShell(boolean paralelo) {
    int ret = 0;
    MShell msh = createMShell();
    msh.parallel = paralelo;
    char input[MAX_INPUT+1];
    do {
        printPrompt(&msh);
        if (fgets(input, MAX_INPUT, stdin)) {
            input[strlen(input)-1] = '\0'; // the fgets does not remove \n from end of string. I need remove manually.
            runCommand(&msh, input);
        } else {
            ret = ERROR_WHEN_READ_INPUT;
            break;
        }
    } while (strcmp(input, "exit"));


    printMShell(&msh);
    deleteMShell(&msh);
    return ret;
}
