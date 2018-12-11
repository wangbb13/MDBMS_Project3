#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "types.hpp"
using namespace std;

template <typename T>
T string2Num(string str) {
    T ans;
    istringstream iss(str);
    iss >> ans;
    return ans;
}

template <typename T>
void readData(const char* filename,
              vector< pair<T, T> >& container,
              T& node_min, T& node_max, 
              int ignore = 4,
              char delimiter = '\t',
              char newline = '\n') {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << filename << " is not exists." << endl;
        return;
    }
    node_min = node_max = 0;
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
        container.push_back(make_pair(pl, pr));
        node_min = min(node_min, pl);
        node_min = min(node_min, pr);
        node_max = max(node_max, pl);
        node_max = max(node_max, pr);
    }
    fin.close();
}