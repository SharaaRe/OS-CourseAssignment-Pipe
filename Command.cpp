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

using namespace std;

Command::Command(string command) {
    stringstream ss(command);
    string token;
    vector <query> qs;
    query q;
    while (getline(ss, token, '-'))
    {
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
        cout << token << endl;
        stringstream ts(token);
        getline(ts, q.first, '=');
        getline(ts, q.second);
        if (q.first == DIR)
            this->dir = q.second;
        else if (q.second == ASC or q.second == DESC)
            this->filter.field_to_sort.push_back(q);
        else
            this->filter.field_names.push_back(q);
    }
}

string Command::get_dir() {
    return dir;
}

Filter* Command::get_filter(string filename) {
    int fd;
    string address = "./" + dir + filename;
    if ((fd = open(address.c_str(), O_RDWR)) > 0) {
        Filter* fil = new Filter;
        *fil = this->filter;
        fil->fd = fd;
        return fil;
    } else 
        return NULL;
}

void Command::print() {
    cout << "directory " << dir << endl; 
    cout << "Fields" << endl;
    for (int i = 0; i < filter.field_names.size(); i++) 
        cout << "first " << filter.field_names[i].first << "- second " << filter.field_names[i].second << endl;

    cout << "Sorts" << endl;
    for (int i = 0; i < filter.field_to_sort.size(); i++) 
        cout << "first " << filter.field_to_sort[i].first << "- second " << filter.field_to_sort[i].second << endl;
        
}