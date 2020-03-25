#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include <dirent.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Command.h"


constexpr int READ_INDEX = 0;
constexpr int WRITE_INDEX = 1;

typedef std::vector <int> pipev;

class LoadBalancer{
private:

    std::vector <std::pair <pid_t, pipev>> childs;
    std::vector <std::string> files_in_dir(std::string dir_name);
    void parent_job(Filter filter, pid_t pid, int p[]);
    void child_job(int p[]);
    void handle_command(Command cmd);

public:
    void run();
};

#endif