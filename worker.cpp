#include <dirent.h>
#include <unistd.h> 
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

constexpr int READ_INDEX = 0;
constexpr int WRITE_INDEX = 1;
constexpr int BUFFER_SIZE = 32;


using namespace std;

int main(int argc, char* argv[]) {
    int p[2];
    char buff[BUFFER_SIZE];
    int nbytes;

    if (argc < 3) {
        cerr << "Few arguments for worker process" << endl;
        exit(EXIT_FAILURE);
    }
    p[WRITE_INDEX] = stoi(argv[1]); 
    p[READ_INDEX] = stoi(argv[2]); 

    if ((nbytes = read(p[READ_INDEX], buff, BUFFER_SIZE)) > 0) {
        cout << "worker process" << buff << endl;
    } else if (nbytes != 0) {
        exit(EXIT_FAILURE);
    }


}