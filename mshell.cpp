#include "mshell.h"
#include <unistd.h>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <limits.h>
#include <map>
#include <sys/wait.h>
#include <sys/stat.h>

using namespace std;

#if __cplusplus > 201402L

#include <filesystem>

#else

#define GET_CURRENT_PATH_ERROR 2

#endif

#define ERROR_WHEN_READ_INPUT 3
#define INTERN_COMMAND_NOT_FOUND 4
#define EMPTY_COMMAND 5

inline bool fileExists (const string& completePath) {
  struct stat buffer;   
  return (stat (completePath.c_str(), &buffer) == 0); 
}

MShell::MShell() : 
    parallel(false),
    command_enable(false),
    file_enable(false),
    lastReturn(0)
{
    variables["PATH"] = "/home/michael/.local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:/opt/X11/bin:/opt/local/bin:/usr/texbin:/usr/local/android-studio/bin:/home/michael/gitProjects/LogAnnalyse/LogAnnalyse:/home/michael/gitProjects/plotCPUStatus:/etc/vysor-3.1.4/:/home/michael/gitProjects/LogAnnalyse/:/home/michael/gitProjects/LogAnnalyse/WebRequests";
    //variables["PATH"] = "/bin/:/usr/local/bin/";
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
    vector<string> params = separateArgs(command);
    if (params.size() > 0) {
        map<string, InternCommand>::iterator it = internCommands.find(params[0]);
        if (it != internCommands.end()) {
            InternCommand func = it->second;
            return (this->*func)(params);
        }
        return INTERN_COMMAND_NOT_FOUND;
    }
    return EMPTY_COMMAND;
}

char ** MShell::convertToArgv(vector<string> params) {
    char ** ret = new char*[params.size()+1];
    for(int i = 0; i < params.size(); i++) {
        ret[i] = new char[params[i].size()+1];
        strcpy(ret[i], params[i].c_str());
    }
    ret[params.size()] = nullptr;
    return ret;
}

char ** MShell::getEnv() {

    char ** ret;

    vector<string> tokenized;
    stringstream path;
    string singlePath;
    path << variables["PATH"];

    while(getline(path, singlePath, ':')) {
        tokenized.push_back(singlePath);
    }
    ret = convertToArgv(tokenized);
#ifdef DEBUG
    for ( int i = 0; ret[i] != nullptr; i++) {
        cout << "EnvRet[" << i << "]: " << ret[i] << endl;
    }
#endif
    return ret;
}

int MShell::runBinary(string command) {
    pid_t pid = fork();
    if (pid == 0) {
        vector<string> params = separateArgs(command);
        if (params.size() > 0) {
#ifdef DEBUG
            for (int i = 0; i < params.size(); i++ ) {
                cout << "params[" << i << "]: " << params[i] << endl;
            }
#endif
            char ** argv = convertToArgv(params);
            //char *argv[] = {"/bin/echo", "echo", "testando", "1234", nullptr};
            char ** env = getEnv();
            //exit(execve(argv[0], argv, this->environ));
#ifdef DEBUG
            for (int i = 0; i < params.size(); i++ ) {
                cout << "argv[" << i << "]: " << argv[i] << endl;
            }
#endif
            for (int i = 0; env[i] != nullptr; i++) {
                string completePath = env[i];
                if (completePath[completePath.size()-1] != '/') {
                    completePath += "/";
                }
                completePath += argv[0];
#ifdef DEBUG
                cout << "Env[" << i << "]: " << env[i] << endl;
                cout << "CompletePath: " << completePath << endl;
                cout << "FileExists: " << fileExists(completePath) << endl;
#endif
                if (fileExists(completePath)) {
                    //argv = &(argv[1]);
                    exit(execve(completePath.c_str(), argv, this->environ));
                }
            }
            exit(execve(argv[0], argv, this->environ));
        } else {
            exit(EMPTY_COMMAND);
        }
    } else {
        int ret;
        wait(&ret);
#ifdef DEBUG
        cout << "When finish Proccess: " << WEXITSTATUS(ret) << endl;
#endif
        return WEXITSTATUS(ret);
    }

    return 0;
}

int MShell::runCommand(string command) {
    int ret = 0;

    stringstream commandStream;
    commandStream << command;
    string intermediateCommand;

    while(getline(commandStream, intermediateCommand, ';')) {
        ret = this->runIntern(intermediateCommand);
#ifdef DEBUG
        cout << "Returnt intern: " << ret << endl;;
#endif
        if (ret) {
            ret = this->runBinary(intermediateCommand);
#ifdef DEBUG
        cout << "Return binary: " << ret << endl;;
#endif
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
#ifdef DEBUG
    cout << (*this) << endl;
#endif
    return ret;
}

int MShell::chdir(vector<string> args) {
    for (int i = 0; i < args.size(); i++) {
        cout << "args[" << i << "]: " << args[i] << endl;
    }
    return 0;
}

int MShell::pwd(vector<string> args) {
    cout << getVariable("PWD");
    return 0;
}

MShell::~MShell() { }

vector<string> MShell::separateArgs(string command) {
    vector<string> args;
    stringstream commandStream;
    commandStream << command;
    string arg;
    do {
        commandStream >> arg;
        args.push_back(arg);
    } while(!commandStream.eof());
    return args;
}

void MShell::setEnv(char ** env) {
    this->environ = env;
}
