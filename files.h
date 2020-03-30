#ifndef FILES_H
#define FILES_H

#include <string>
#include <vector>
#include <map>

// class FileData{
// public:
//     FileData(std::vector <std::vector <std::string>>);

// private:
//     map <std::string, int> fields_index;
//     std::vector <std::vector <std::string>>  data;
// };

std::vector <std::string> split(std::string s, char delim);
std::vector <std::vector<std::string>> parse_file(std::string filename);
std::vector <std::vector<std::string>> parse_data(std::string data);
std::string serialize(std::vector <std::vector <std::string>> file_data);

#endif
