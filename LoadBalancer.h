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

constexpr int READ_INDEX = 0;
constexpr int WRITE_INDEX = 1;


class LoadBalancer{
private:
    std::vector <std:: vector <int>> pipes;
    std::vector <pid_t> childs;

public:
    std::vector <std::string> files_in_dir(std::string dir_name);
    void parent_job(std::string filename, pid_t pid, int p[]);
    void child_job()
};

#endif