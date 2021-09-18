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
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>
#include"mshell.h"

using namespace std;

int main(int argc, char *argv[]) {
    bool paralelo = false;
    MShell msh;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp("-p", argv[i]) == 0 || strcmp("--paralelo", argv[i]) == 0) {
                paralelo = true;
            } else {
                printf("Wrong Param: %s\n", argv[i]);
                exit(1);
            }
        }
    }
    return msh.run();
}
