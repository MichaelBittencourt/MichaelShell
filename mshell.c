#include"mshell.h"
#include<stdio.h>

void runMShell(boolean paralelo) {
    if (paralelo) {
        printf("RunShell Paralelo!\n");
    } else {
        printf("RunShell Sequencial!\n");
    }
}
