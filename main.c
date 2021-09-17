/*
 *
 * Interpretar o parametros
 *
 * Executar o shell
 *   - Ler input do usuário
 *   - Executar o comando
 *     * Se o comando for nativo do shell, a gente roda a função interna
 *     * Caso contrário a gente tenta localizar o binário e executar ele.
 *   - repete até digitar exit
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"mshell.h"

int main(int argc, char *argv[]) {
    boolean paralelo = FALSE;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp("-p", argv[i]) == 0 || strcmp("--paralelo", argv[i]) == 0) {
                paralelo = TRUE;
            } else {
                printf("Wrong Param: %s\n", argv[i]);
                exit(1);
            }
        }
    }
    return runMShell(paralelo);
}
