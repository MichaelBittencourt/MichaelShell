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

#define VERSION "0.0.1"

using namespace std;

void showHelpMessage(char * arg0) {
    printf("Usage: %s [-h | -v | -c command | -f file.sh]\n", arg0);
    printf("\t-h | --help\tShow help message\n");
    printf("\t-v | --version\tShow msh version\n");
    printf("\t-p | --parallel\tRun command passed as parameter\n");
    printf("\t-c | --command\tRun command passed as parameter\n");
    printf("\t-f | --file.sh\tRun command passed as parameter\n");
}

void showVersion() {
    printf("Version: %s\n", VERSION);
}

int main(int argc, char *argv[]) {
    bool parallel = false;
    bool command = false;
    bool file = false;
    MShell msh;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp("-p", argv[i]) == 0 || strcmp("--parallel", argv[i]) == 0) {
                parallel = true;
            } else if (strcmp("-c", argv[i]) == 0 || strcmp("--command", argv[i]) == 0) {
                command = true;
                printf("Not Implemented yet, --command option. Param: %s", argv[++i]);
            } else if (strcmp("-f", argv[i]) == 0 || strcmp("--file", argv[i]) == 0) {
                file = true;
                printf("Not Implemented yet, --file option. Param: %s", argv[++i]);
            } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
                showVersion();
                exit(0);
            } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                showHelpMessage(argv[0]);
                exit(0);
            } else {
                printf("Wrong Param: %s\n", argv[i]);
                exit(1);
            }
        }
    }
    return msh.run();
}
