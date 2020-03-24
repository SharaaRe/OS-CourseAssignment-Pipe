#include <dirent.h>
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
    vector <string> file_names;;
    while (getline(cin, command)) {
        // parse_input
        // open_dir
        file_names = files_in_dir(command);
        sort(file_names.begin(), file_names.end(), compare);
        for (string filename : file_names){
            cout << filename << endl;
        }
        

    }
}