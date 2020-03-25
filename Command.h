#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <utility>
#include <vector>

typedef std::pair <std::string, std::string> query;


struct  Filter
{
    int fd;
    std::vector <query> field_names;
    std::vector <query> field_to_sort;
};


class Command {
private:
    int n_processes;
    std::string dir;
    Filter filter;
    void parse(std::string command);
public:
    Command(std::string command_input);
    Filter* get_filter(std::string filename);
    std::string get_dir();
    void print();
};

#endif