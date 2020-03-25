#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <utility>
#include <vector>

#define QUIT "quit"

typedef std::pair <std::string, std::string> query;


typedef struct  filter
{
    std::string pipe_name;
    std::vector <query> field_names;
    std::vector <query> field_to_sort;
}Filter;

void print_filter(Filter fil);

class Command {
private:
    int n_processes;
    std::string dir;
    Filter filter;
    void parse(std::string command);
public:
    Command(std::string command_input);
    Filter get_filter();
    std::string get_dir();
    int get_n();
    void print();
};

#endif