#pragma once
#include <fstream>
#include <sstream>
#include "types.hpp"
using namespace std;

#define INSERT
#define REMOVE
#define ALGUPDATE
#define ALGSUP
#define ALGDECOMP
#define ALGBUILD

template <typename T>
T string2Num(string str) {
    T ans;
    istringstream iss(str);
    iss >> ans;
    return ans;
}

template <typename T>
void readData(const char* filename,
              Graph<T>& graph, 
              int ignore = 4,
              char delimiter = '\t',
              char newline = '\n') {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << filename << " is not exists." << endl;
        return;
    }
    string str_line = "";
    string str_num = "";
    istringstream iss(str_line);
    T pl, pr;
    for (uint _ = 0; _ < ignore; ++ _)
        getline(fin, str_line, newline);
    while (getline(fin, str_line, newline)) {
        iss.clear();
        iss.str(str_line);
        getline(iss, str_num, delimiter);
        pl = string2Num<T>(str_num);
        getline(iss, str_num, delimiter);
        pr = string2Num<T>(str_num);
        graph.add(pl, pr);
    }

    graph.endInsert();
    fin.close();
}

template <typename T>
string edgeStr(T x, T y) {
    string ans = "";
    string u = to_string(x);
    string v = to_string(y);
    if (x < y) 
        ans = u + "-" + v;
    else
        ans = v + "-" + u;
    return ans;
}
