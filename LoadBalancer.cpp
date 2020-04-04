#include "LoadBalancer.h"

#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <fstream>

#define EXP -1
#define WORKER "worker.out"

using namespace std;

bool junk(string s) {return s[0] == '.';}

bool compare(string a, string b) {
    return a < b;
}

vector <string> LoadBalancer::files_in_dir(string dir_name) {
    vector <string> files;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dir_name.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            files.push_back(ent->d_name);
        }
    closedir (dir);
    return files;
    } else {
        throw EXP;

    }
}

void LoadBalancer::parent_job(const char* cmd, pid_t pid, int p[]) {

    vector <int> pv;
    pair <pid_t, pipev> child;
    pv.push_back(p[READ_INDEX]);
    pv.push_back(p[WRITE_INDEX]);
    child.first = pid;
    child.second = pv;
    childs.push_back(child);
    write(pv[WRITE_INDEX], cmd, PIPE_BUF);
}

void LoadBalancer::child_job(int p[], const char* fifo){

    char read_[10], write_[10];
    strcpy(read_, to_string(p[READ_INDEX]).c_str());
    strcpy(write_, to_string(p[WRITE_INDEX]).c_str()); 
    string address = string("./") + WORKER;
    execlp(address.c_str(), WORKER, read_, write_, fifo, NULL);
}


void LoadBalancer::devide_works(vector <string> file_names, int n_process) {

    auto end = remove_if(file_names.begin(), file_names.end(), junk);
    file_names.erase(end, file_names.end());
    char buff[PIPE_BUF];

    for (int i = 0; i < file_names.size(); i++) {
        memset(buff, 0, PIPE_BUF);
        strcpy(buff, file_names[i].c_str());
        pair <pid_t, pipev> child = childs[i % n_process];
        write(child.second[WRITE_INDEX], buff, PIPE_BUF);
    }
}

void LoadBalancer::finish_works() {

    char buff[PIPE_BUF] = {0};
    for (int i = 0; i < childs.size(); i++) {
        int stat;
        memset(buff, 0, PIPE_BUF);
        strcpy(buff, QUIT);
        pair <pid_t, pipev> child = childs[i];
        write(child.second[WRITE_INDEX], QUIT, strlen(QUIT) + 1);
        waitpid(child.first, &stat, 0);
    }
}

void LoadBalancer::repres_job(Command cmd, string fifo, int fd) {

    char buff[PIPE_BUF] = {0};
    mkfifo(fifo.c_str(), 0666);

    pid_t pid = fork();
    string REP = "representer.out";
    if (pid > 0) {
        this->repres = pid;
 
        strcpy(buff, cmd.get_serialized().c_str());
        write(fd, buff, PIPE_BUF);

    } else if (pid == 0) {
        string addr = "./" + REP;
        execlp(addr.c_str(), REP.c_str(), fifo.c_str(), NULL);
    }
}
void LoadBalancer::handle_command(Command cmd) {
    vector <string> file_names;
    int fd, stat;
    int p[2];
    pid_t pid;
    char buff[PIPE_BUF] = {0};
    n_process = cmd.get_n();

    string fifo = "./" + cmd.get_dir() + "_fifo";
    fd = open(fifo.c_str(), 0666);
    repres_job(cmd, fifo, fd);
    strcpy(buff, cmd.get_serialized().c_str());

    for (int i = 0; i < n_process; i++) {
        pipe(p);
        pid = fork();
        if (pid > 0) {
            parent_job(buff, pid, p);
        } else if (pid == 0) {
            child_job(p, fifo.c_str());
        }
    }
    try {
        file_names = files_in_dir(cmd.get_dir());
    } catch (int e){
        cout << "directory not found!" << endl;
        childs.clear();
        return;
    }
    devide_works(file_names, n_process);
    finish_works();

    strcpy(buff, QUIT);
    write(fd, buff, PIPE_BUF);
    waitpid(repres, &stat, 0);
    childs.clear();
    close(fd);

}


void LoadBalancer::run() {
    string command;
    while (getline(cin, command)) {
        if (command == QUIT)
            return;
        Command cmd(command);
        handle_command(cmd);
    }
}

