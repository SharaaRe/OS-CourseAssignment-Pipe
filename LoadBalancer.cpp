#include "LoadBalancer.h"

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

void LoadBalancer::parent_job(string filename, pid_t pid, int p[]) {
    vector <int> pv;
    pv.push_back(p[READ_INDEX]);
    pv.push_back(p[WRITE_INDEX]);
    pipes.push_back(pv);
    childs.push_back(pid);
    write(p[WRITE_INDEX], filename.c_str(), filename.length());
    cout << "parent" << "-" << filename << "-" << p[WRITE_INDEX] << "-" << p[READ_INDEX] << endl;
}

void LoadBalancer::child_job(int p[]){
    char read_[5], write_[5];
    strcpy(read_, to_string(p[READ_INDEX]).c_str());
    strcpy(write_, to_string(p[WRITE_INDEX]).c_str()); 
    cout << "child fork" << endl;
    char* args[] = {"./worker.out", read_, write_};
    execv(args[0], args);
}

void LoadBalancer::run() {
    string command;
    vector <string> file_names;
    vector <vector <int>> pipes;
    vector <pid_t> childes;
    int p[2];
    while (getline(cin, command)) {
        file_names = files_in_dir(command);
        sort(file_names.begin(), file_names.end(), compare);

        for (int i; i < file_names.size(); i++){
            string filename = file_names[i];
            pid_t pid;
            pipe(p);

            pid = fork();
            if (pid > 0) {
                parent_job(filename, pid, p);
            } else if (pid == 0) {
                child_job(p);
            }

        }
        

    }
}

