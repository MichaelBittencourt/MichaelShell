#include "mshell.h"
#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>
#include <limits.h>
#include <map>

using namespace std;

#if __cplusplus > 201402L

#include <filesystem>

#else

#define GET_CURRENT_PATH_ERROR 2

#endif

#define ERROR_WHEN_READ_INPUT 3
#define INTERN_COMMAND_NOT_FOUND 4

MShell::MShell() : 
    parallel(false),
    lastReturn(0)
{
    variables["PATH"] = "/bin/";
    variables["PS1"] = "$> ";
#if __cplusplus > 201402L 
    variables["PWD"] = std::filesystem::current_path();
#else
    char pwd[PATH_MAX+1];
    if (getcwd(pwd, sizeof(pwd)) == NULL) {
        exit(GET_CURRENT_PATH_ERROR);
    }
    string cpppwd(pwd);
    variables["PWD"] = pwd;
#endif
    internCommands["cd"] = &MShell::chdir;
    internCommands["pwd"] = &MShell::pwd;
}

void MShell::showPrompt() {
    cout << endl << getVariable("PS1");
}

string MShell::getVariable(string variable) {
    return variables[variable];
}

ostream& operator<<(ostream& os, const MShell& mshell)
{
    string tab = "    ";
    os << "MShell settings:" << endl;
    os << tab << "All Shell variables:" << endl;
    for (map<string, string>::const_iterator it = mshell.variables.begin(); it != mshell.variables.end(); it++) {
        os << tab << tab << it->first << ": " << it->second << endl;
    }
    os << tab << "All Intern commands:" << endl;
    for (map<string, InternCommand>::const_iterator it = mshell.internCommands.begin(); it != mshell.internCommands.end(); it++) {
        os << tab << tab << it->first << ": " << it->second << endl;
    }
    return os;
}

int MShell::runIntern(string command) {
    //cout << endl << "Running Intert command: " << command;
    map<string, InternCommand>::iterator it = internCommands.find(command);
    if (it != internCommands.end()) {
        vector<string> params;
        InternCommand func = it->second;
        return (this->*func)(params);
    }
    return INTERN_COMMAND_NOT_FOUND;
}

int MShell::runBinary(string command) {
    cout << endl << "Running binary: " << command;;
    return 0;
}

int MShell::runCommand(string command) {
    int ret = 0;

    stringstream commandStream;
    commandStream << command;
    string intermediateCommand;

    while(getline(commandStream, intermediateCommand, ';')) {
        ret = this->runIntern(intermediateCommand);
        if (ret) {
            ret = this->runBinary(intermediateCommand);
        }
    }
    return ret;
}

int MShell::run() {
    int ret = 0;
    string input;
    do {
        this->showPrompt();
        getline(cin, input);
        if (!cin.fail()) {
            this->runCommand(input);
        } else {
            ret = ERROR_WHEN_READ_INPUT;
            break;
        }
    } while (input != "exit");
    cout << (*this) << endl;
    return ret;
}

int MShell::chdir(vector<string> args) {
    return 0;
}

int MShell::pwd(vector<string> args) {
    cout << getVariable("PWD");
    return 0;
}

MShell::~MShell() { }
