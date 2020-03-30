#ifndef COMMAND_H
#define COMMAND_H



#include <string>
#include <utility>
#include <vector>
typedef std::pair <std::string, std::string> query;

#define QUIT "quit"
// #define BUFFER_SIZE 1024



std::vector <std::string> split(std::string s, char delim);

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
    std::string serialized;
    std::string dir;
    Filter filter;
    void parse(std::string command);
public:
    // static std::string serialize(Command cmd);
    // static Command deserialize (std::string cmd);
    Command(std::string command_input);
    Filter get_filter();
    std::string get_dir();
    std::string get_serialized();
    int get_n();
    void print();
};

#endif