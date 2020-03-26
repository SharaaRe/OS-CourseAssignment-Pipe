#include <dirent.h>
#include <unistd.h> 
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "Command.h"

constexpr int READ_INDEX = 0;
constexpr int WRITE_INDEX = 1;
constexpr int BUFFER_SIZE = 32;


using namespace std;

vector <vector<string>> parse_file(string filename) {
    fstream fs;
    string line;
    vector <vector <string>> file_data;
    // vector <map <string, string>> res;[]
    fs.open(filename);
    while (getline(fs, line))
    {
        // cout << line << endl;
        file_data.push_back(split(line, '-'));
    }

    return file_data;
}

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
    // cout << "filename " << filename << endl; 
    for (int i = 0; i < file_data[0].size(); i++) {
        index.insert(pair <string, int> (file_data[0][i], i));
        // cout << file_data[0][i] << endl;
    }
    

    for (int i = 0; i < fil.field_names.size(); i++) {
        // cout << fil.field_names.size() << endl;
        if (index.find(fil.field_names[i].first) != index.end()) {
            // cout << "filter by " << fil.field_names[i].first << " value must be " << fil.field_names[i].second << endl;
            int j = index[fil.field_names[i].first];
            string value = fil.field_names[i].second;
            // filter_by(j, fil.field_names[i].second, file_data);
            auto pend = remove_if(file_data.begin(), file_data.end(), [j, value](vector <string>& a){return a[j] != value;});
            file_data.erase(pend, file_data.end());
        }
    }

    return file_data;
    // for (int i = 0; i < fil.)
}

int main(int argc, char* argv[]) {
    int p[2];
    Command cmd("");
    char buff[BUFF_SIZE] = {0};
    int nbytes;
    // cout << "worker.cpp executed" << endl;
    if (argc < 3) {
        cerr << "Few arguments for worker process" << endl;
        exit(EXIT_FAILURE);
    }
    p[READ_INDEX] = stoi(argv[1]); 
    p[WRITE_INDEX] = stoi(argv[2]); 

    nbytes = read(p[READ_INDEX], buff, BUFF_SIZE);
    if (nbytes > 0) {
        // cout << "worker process" << buff << endl;
        cmd = Command(string(buff));
        // cmd.print();
    }



    while((nbytes = read(p[READ_INDEX], buff, BUFF_SIZE)) > 0) {
        if (strcmp(buff, "quit") != 0){
            cout << buff << endl;
            vector <vector <string>> file_data = filter_file(string(buff), cmd);
            // cout << "file size" << file_data.size() << endl;
            for (int i = 0; i < file_data.size(); i++) {
                for (int j = 0; j < file_data[i].size(); j++)
                    cout << file_data[i][j] << " / ";
                cout << endl;
            }
            
            memset(buff, 0, BUFF_SIZE);
        } else 
            exit(EXIT_SUCCESS);
    }

}