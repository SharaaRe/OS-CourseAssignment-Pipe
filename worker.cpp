#include <dirent.h>
#include <unistd.h> 
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Command.h"

constexpr int READ_INDEX = 0;
constexpr int WRITE_INDEX = 1;
constexpr int BUFFER_SIZE = 32;


using namespace std;

int main(int argc, char* argv[]) {
    int p[2];
    Filter fil;
    int nbytes;
    // cout << "worker.cpp executed" << endl;
    if (argc < 3) {
        cerr << "Few arguments for worker process" << endl;
        exit(EXIT_FAILURE);
    }
    p[READ_INDEX] = stoi(argv[1]); 
    p[WRITE_INDEX] = stoi(argv[2]); 



    while((nbytes = read(p[READ_INDEX], &fil, sizeof(fil))) <= 0) {
        continue;
    }
    cout << "worker process"  << endl;
    cout << fil.field_names.size() << endl;
    // print_filter(fil);
    cout << "Fields" << endl;
    for (int i = 0; i < fil.field_names.size(); i++) 
        cout << "first " << fil.field_names[i].first << "- second " << fil.field_names[i].second << endl;

    cout << "Sorts" << endl;
    for (int i = 0; i < fil.field_to_sort.size(); i++) 
        cout << "first " << fil.field_to_sort[i].first << "- second " << fil.field_to_sort[i].second << endl;

}