#ifndef M_SHELL_H
#define M_SHELL_H
#include<map>
#include<string>
#include<vector>

using namespace std;

class MShell;

typedef int (MShell::*InternCommand)(vector<string>);

class MShell {

    public:

        MShell();
        ~MShell();
        int run();
        void enableParallel();
        void disableParallel();
        friend ostream& operator<<(ostream& os, const MShell& mshell);

    private:

        map<string, string> variables;
        map<string, InternCommand> internCommands;
        bool parallel;
        int lastReturn;
        void showPrompt();
        int chdir(vector<string> args);
        int pwd(vector<string> args);
        int runIntern(string command);
        int runBinary(string command);
        int runCommand(string command);
        string getVariable(string variable);
        vector<string> separateArgs(string command);
        char ** convertToArgv(vector<string> params);
        char ** getEnv();

};

#endif
