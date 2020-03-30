#include "files.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>


using namespace std;


vector <string> split(string s, char delim) {
    vector <string> splited;
    string token;
    stringstream ss(s);
    while (getline(ss, token, delim)){
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
        splited.push_back(token);
    }
    return splited;
}

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


vector <vector<string>> parse_data(string data) {
    string line;
    vector <vector <string>> file_data;
    stringstream ss(data);
    // vector <map <string, string>> res;[]
    while (getline(ss, line))
    {
        // cout << line << endl;
        file_data.push_back(split(line, '-'));
    }

    return file_data;
}

string serialize(vector <vector <string>> file_data) {
    stringstream ss;
    for (int i = 0; i < file_data.size(); i++) {
        for (int j = 0; j < file_data[i].size(); j++) {
            ss << file_data[i][j];
            if (j != file_data[i].size() - 1) {ss << " - ";}
        }
        ss << endl;
    }
    return ss.str();
}