#include <dirent.h>
#include <unistd.h> 
#include <string.h>
#include <fcntl.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "Command.h"
#include "files.h"

constexpr int READ_INDEX = 0;
constexpr int WRITE_INDEX = 1;
constexpr int BUFFER_SIZE = 32;


using namespace std;


void filter_by (int key, string value, vector <vector<string>> &file_data) {
    for (auto it = file_data.begin(), last = file_data.end(); it != last;   ) 
        if ((*it)[key] != value)  
             it = file_data.erase(it);
        else
             it++;
}



vector <vector<string>> filter_file(string filename, Command cmd) {
    map <string, int> index;
    Filter fil = cmd.get_filter();
    filename = "./" + cmd.get_dir() + "/" + filename;
    vector <vector <string>> file_data = parse_file(filename);
    for (int i = 0; i < file_data[0].size(); i++) {
        index.insert(pair <string, int> (file_data[0][i], i));
    }

    for (int i = 0; i < fil.field_names.size(); i++) {

        if (index.find(fil.field_names[i].first) != index.end()) {
            int j = index[fil.field_names[i].first];
            string value = fil.field_names[i].second;
            auto pend = remove_if(file_data.begin() + 1, file_data.end(), [j, value](vector <string>& a){return a[j] != value;});
            file_data.erase(pend, file_data.end());
        }
    }

    return file_data;
    // for (int i = 0; i < fil.)
}

int main(int argc, char* argv[]) {
    int p[2];
    Command cmd("");
    string fifo;
    char buff[PIPE_BUF] = {0};
    int nbytes;
    // cout << "worker.cpp executed" << endl;
    if (argc < 4) {
        cerr << "Few arguments for worker process" << endl;
        exit(EXIT_FAILURE);
    }
    p[READ_INDEX] = stoi(argv[1]); 
    p[WRITE_INDEX] = stoi(argv[2]); 
    fifo = string(argv[3]);

    int fd = open(fifo.c_str(), 0666);

    nbytes = read(p[READ_INDEX], buff, PIPE_BUF);
    if (nbytes > 0 && strlen(buff) > 0) {
        cmd = Command(string(buff));
    } else {
        exit(EXIT_FAILURE);
    }



    while((nbytes = read(p[READ_INDEX], buff, PIPE_BUF)) > 0) {
        if (strcmp(buff, "quit") != 0){
            // cout << buff << endl;
            vector <vector <string>> file_data = filter_file(string(buff), cmd);
            // cout << "file size" << file_data.size() << endl;
            if (file_data.size() > 1) {

                // cout << serialize(file_data);
                memset(buff, 0, PIPE_BUF);

                strcpy(buff, serialize(file_data).c_str());
                write(fd, buff, PIPE_BUF);
                memset(buff, 0, PIPE_BUF);
            }
        } else 
            exit(EXIT_SUCCESS);
    }

}