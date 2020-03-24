#include <dirent.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>



constexpr int READ_INDEX = 0;
constexpr int WRITE_INDEX = 1;


using namespace std;

vector <string> files_in_dir(string dir_name) {
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

bool compare(string a, string b) {
    return a < b;
}

int main() {
    string command;
    vector <string> file_names;
    vector <vector <int>> pipes;
    vector <pid_t> childes;
    int p[2];
    while (getline(cin, command)) {
        // parse_input
        // open_dir
        file_names = files_in_dir(command);
        sort(file_names.begin(), file_names.end(), compare);
        for (string filename : file_names){
            pid_t pid;
            // cout << filename << endl;
            pipe(p);
            
            pid = fork();
            if (pid > 0) {
                // parent process
                vector <int> pv;
                pv.push_back(p[READ_INDEX]);
                pv.push_back(p[WRITE_INDEX]);
                pipes.push_back(pv);
                childes.push_back(pid);
                write(p[WRITE_INDEX], filename.c_str(), filename.length());
                cout << "parent fork" << endl;

            } else if (pid == 0) {
                // child process
                char read_[5], write_[5];
                strcpy(read_, to_string(p[READ_INDEX]).c_str());
                strcpy(write_, to_string(p[WRITE_INDEX]).c_str()); 
                cout << "child fork" << endl;
                char* args[] = {"./worker.out", read_, write_};
                execv(args[0], args);

            }

        }
        

    }
}