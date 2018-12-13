#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
using namespace std;

typedef unsigned int uint;

/* undirected */
template <typename T>
class Edge {
public:
    T a;
    T b;
    string str;
    string _str_1;
    Edge(T x, T y) {
        a = min(x, y);
        b = max(x, y);
        string u = to_string(a);
        string v = to_string(b);
        str = u + "-" + v;
        _str_1 = v + "-" + u;
    }
    ~Edge() {}
};

template <typename T>
ostream& operator << (ostream& out, const Edge<T>& e) {
    out << e.a << "--" << e.b << " ";
    return out;
}

/* undirected */
template <typename T>
class Graph {
public:
    vector< Edge<T> > edge_list;
    unordered_map<string, uint> edge_index_map;
    vector< unordered_set<T> > adj;
    vector< uint > degree;       // node degree
    vector< uint > edge_support; // edge support
    vector< uint > edge_tau;     // edge trussness

    uint node_min_id;
    uint node_max_id;
    uint node_number;
    uint edge_number;
    uint max_edge_tau;
    uint min_edge_tau;

    Graph() {
        node_min_id = 0;
        node_max_id = 0;
        node_number = 0;
        edge_number = 0;
    }
    
    ~Graph() {}
    
    void add(T x, T y) {
        Edge<T> e(x, y);
        edge_list.push_back(e);
        edge_index_map[e.str] = edge_number ++;
        node_min_id = min(node_min_id, x);
        node_min_id = min(node_min_id, y);
        node_max_id = max(node_max_id, x);
        node_max_id = max(node_max_id, y);
    }
    
    void end() {
        node_number = node_max_id - node_min_id + 1;
        adj.resize(node_number + 1);
        degree.resize(node_number + 1);
        fill(degree.begin(), degree.end(), 0);
        for (auto& e : edge_list) {
            adj[e.a].insert(e.b);
            adj[e.b].insert(e.a);
            degree[e.a] ++;
            degree[e.b] ++;
        }
        edge_support.resize(edge_number);
        edge_tau.resize(edge_number);
        fill(edge_support.begin(), edge_support.end(), 0);
        fill(edge_tau.begin(), edge_tau.end(), 0);
    }
};

class SuperGraph
{
public:
    vector< unordered_set<uint> > super_vertex;
    vector<uint> vertex_truss;
    vector< Edge<uint> > edge_list;
    vector< unordered_set<uint> > adj;

    uint super_node_number;
    uint super_edge_number;

    SuperGraph() {
        super_node_number = 0;
        super_edge_number = 0;
    }

    void newVertex(uint k) {
        super_vertex.push_back( unordered_set<uint>() );
        vertex_truss.push_back(k);
        super_node_number ++;
    }

    void addVertex(uint edge_no) {
        super_vertex[super_node_number - 1].insert(edge_no);
    }

    void newEdge(uint u, uint v) {
        edge_list.push_back(Edge<uint>(u, v));
        super_edge_number ++;
    }

    void end() {
        adj.resize(super_node_number + 1);
        for (auto& e : edge_list) {
            adj[e.a].insert(e.b);
            adj[e.b].insert(e.a);
        }
    }

    ~SuperGraph() {}
};
