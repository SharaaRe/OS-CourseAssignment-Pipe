#include "Command.h"

#include <stdio.h>
#include <fcntl.h>

#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>


#define DIR "dir"
#define ASC "ascending"
#define DESC "descending"
#define PRC "processes"

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

Command::Command(string command) {
    this->serialized = command;
    stringstream ss(command);
    string token;
    vector <query> qs;
    query q;
    while (getline(ss, token, '-'))
    {
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
        // cout << token << endl;
        stringstream ts(token);
        getline(ts, q.first, '=');
        getline(ts, q.second);
        if (q.first == DIR)
            this->dir = q.second;
        else if (q.second == ASC or q.second == DESC)
            this->filter.field_to_sort.push_back(q);
        else if (q.first == PRC)
            this->n_processes = stoi(q.second);
        else
            this->filter.field_names.push_back(q);
    }
}

string Command::get_serialized() {
    return serialized;
}

string Command::get_dir() {
    return dir;
}

int Command::get_n() {
    return n_processes;
}
Filter Command::get_filter() {
    return filter;
}

void print_filter(Filter fil) {
    cout << "Fields" << endl;
    for (int i = 0; i < fil.field_names.size(); i++) 
        cout << "first " << fil.field_names[i].first << "- second " << fil.field_names[i].second << endl;

    cout << "Sorts" << endl;
    for (int i = 0; i < fil.field_to_sort.size(); i++) 
        cout << "first " << fil.field_to_sort[i].first << "- second " << fil.field_to_sort[i].second << endl;
}

void Command::print() {
    cout << "directory " << dir << endl; 
    print_filter(filter);
 
}