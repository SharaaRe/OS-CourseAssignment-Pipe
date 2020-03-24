#include "LoadBalancer.h"

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

void child_job(int p[]){
    char read_[5], write_[5];
    strcpy(read_, to_string(p[READ_INDEX]).c_str());
    strcpy(write_, to_string(p[WRITE_INDEX]).c_str()); 
    cout << "child fork" << endl;
    char* args[] = {"./worker.out", read_, write_};
    execv(args[0], args);
}