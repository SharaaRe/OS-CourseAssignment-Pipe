#include <sstream>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <utility>



using namespace std;

typedef pair<string, string> query;

int main() {
    string command;
    while(getline(cin, command)) {
        stringstream ss(command);
        string token;
        query q;
        while (getline(ss, token, '-'))
        {
            token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
            stringstream ts(token);
            getline(ts, q.first, '=');
            getline(ts, q.second);
            cout << "first " << q.first << "- second " << q.second << endl;
        }
    }
}