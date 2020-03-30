#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <string.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <limits.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>

#include "files.h"
#include "Command.h"

using namespace std;

void sort_by_filters(Command& cmd, vector <vector <string>>& data) {
    Filter fil = cmd.get_filter();
    bool ascending;
    string field;
    for (int i = 0; i < fil.field_to_sort.size(); i++) {

        field = fil.field_to_sort[i].first;
        auto it = find(data[0].begin(), data[0].end(), field);
        int index = distance(data[0].begin(), it);
        ascending = fil.field_to_sort[i].second == "ascending" ? true: false;
        sort(data.begin() + 1, data.end(), [index, ascending](vector <string> a, vector <string> b) 
                {if (ascending) { return stof(a[index] )< stof(b[index]);}else {return stof(a[index])> stof(b[index]);}});
    }
}


using namespace std;
int main(int argc, char* argv[]) {
    string fifo_name;
    Command cmd("");
    stringstream ss;
    char buff[PIPE_BUF];
    vector <vector <string>> file_data;
    vector <vector <string>> new_data;
    
    int nbytes;
    if (argc < 2) {
        cerr << "representer has few arguments" << endl;
        exit(EXIT_FAILURE);
    } else {
        fifo_name = string(argv[1]);
    }

    int fd = open(argv[1], 0666);
    if ((nbytes = read(fd, buff, PIPE_BUF)) > 0)
        cmd = Command(string(buff));
    else 
        exit(EXIT_FAILURE);
    

    while (true) {
        if((nbytes = read(fd, buff, PIPE_BUF)) > 0) {
            if (strcmp(buff, QUIT) != 0) {
                int start = file_data.size() > 0? 1: 0;
                new_data = parse_data(string(buff));  
                file_data.insert(file_data.end(), new_data.begin() + start, new_data.end());

            } else {
                sort_by_filters(cmd, file_data);
                cout << serialize(file_data);
                exit(EXIT_SUCCESS);
            } 
        }
    }
}