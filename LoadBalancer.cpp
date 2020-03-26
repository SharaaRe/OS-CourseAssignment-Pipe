#include "LoadBalancer.h"


#define WORKER "worker.out"

using namespace std;

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
        /* could not open directory */
        perror ("direction not found!");
        exit(EXIT_FAILURE);
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
    // cout << "buffer in parent job" << cmd << endl;

    // cout << "eh" << endl;
    write(pv[WRITE_INDEX], cmd, BUFF_SIZE);
    // cout << "parent" << "-" << filter.fd << "-" << p[WRITE_INDEX] << "-" << p[READ_INDEX] << endl;
}

void LoadBalancer::child_job(int p[]){
    char read_[10], write_[10];
    strcpy(read_, to_string(p[READ_INDEX]).c_str());
    strcpy(write_, to_string(p[WRITE_INDEX]).c_str()); 
    // cout << "child fork" << endl;
    string address = string("./") + WORKER;
    execlp(address.c_str(), WORKER, read_, write_, NULL);
}

bool junk(string s) {return s[0] == '.';}

void LoadBalancer::handle_command(Command cmd) {
    int n_proccess = cmd.get_n();
    int p[2];
    pid_t pid;
    vector <string> file_names = files_in_dir(cmd.get_dir());
    remove_if(file_names.begin(), file_names.end(), junk);
    // Filter fil = cmd.get_filter();
    sort(file_names.begin(), file_names.end(), compare);
    char buff[BUFF_SIZE] = {0};
    strcpy(buff, cmd.get_serialized().c_str());
    for (int i = 0; i < n_proccess; i++) {
        pipe(p);
        pid = fork();
        if (pid > 0) {
            parent_job(buff, pid, p);
        } else if (pid == 0) {
            child_job(p);
        }
    }
    for (int i = 0; i < file_names.size(); i++) {
        // cout << "first job done" << endl;
        memset(buff, 0, BUFF_SIZE);
        strcpy(buff, file_names[i].c_str());
        // cout << i % n_proccess;
        pair <pid_t, pipev> child = childs[i % n_proccess];
        write(child.second[WRITE_INDEX], buff, BUFF_SIZE);
    }

    for (int i = 0; i < childs.size(); i++) {
        int stat;
        // cout << "quiting job" << endl;
        memset(buff, 0, BUFF_SIZE);
        strcpy(buff, QUIT);
        pair <pid_t, pipev> child = childs[i];
        write(child.second[WRITE_INDEX], QUIT, strlen(QUIT) + 1);
        waitpid(child.first, &stat, 0);
        if (stat == EXIT_SUCCESS)
            cout << "worker exited successfully" << endl;
        else
            cout << "worker failed" << endl;
    }

}

void LoadBalancer::run() {
    string command;
    while (getline(cin, command)) {
        Command cmd(command);
        handle_command(cmd);
        cout << "end handle" << endl;
    }
}

