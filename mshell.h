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
        void enableCommand();
        void enableFile();
        void disableParallel();
        void disableCommand();
        void disableFile();
        void setEnv(char ** env);
        char ** getEnv();
        friend ostream& operator<<(ostream& os, const MShell& mshell);

    private:

        map<string, string> variables;
        map<string, InternCommand> internCommands;
        char ** environ = nullptr;
        bool parallel;
        bool command_enable;
        bool file_enable;
        string file;
        string command;
        int lastReturn;
        void showPrompt();
        int cd(vector<string> args);
        int pwd(vector<string> args);
        int runIntern(string command);
        int runBinary(string command);
        int runCommand(string command);
        string getVariable(string variable);
        void setVariable(string variable, string value);
        vector<string> separateArgs(string command);
        char ** convertToArgv(vector<string> params);
        vector<string> serializePath();
};

#endif
