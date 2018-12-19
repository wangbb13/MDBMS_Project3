#pragma once
#include <set>
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
    
    vector< bool > edge_tomb;    // for dynamic update, false means exists
    vector< bool > affected_node;// literaly
    vector< bool > affected_edge;// literaly

    uint node_min_id;
    uint node_max_id;
    uint node_number;
    uint edge_number;   // max edge_index + 1, precisely
    uint max_edge_tau;
    uint min_edge_tau;

    Graph() {
        node_min_id = 0;
        node_max_id = 0;
        node_number = 0;
        edge_number = 0;
    }
    
    ~Graph() {}
    
    uint add(T x, T y) {    // return edge index, for dynamic
        uint rtn = edge_number;
        Edge<T> e(x, y);
        if (edge_index_map.find(e.str) == edge_index_map.end()) {
            edge_list.push_back(e);
            edge_tomb.push_back(false);
            edge_index_map[e.str] = edge_number ++;
            node_min_id = min(node_min_id, x);
            node_min_id = min(node_min_id, y);
            node_max_id = max(node_max_id, x);
            node_max_id = max(node_max_id, y);
        } else {
            edge_tomb[edge_index_map[e.str]] = false;
        }
        // for dynamic adj and degree
        if (node_max_id + 1 > degree.size()) {
            degree.resize(node_max_id + 1);
        }
        if (node_max_id + 1 > adj.size()) {
            adj.resize(node_max_id + 1);
        }
        adj[x].insert(y);
        adj[y].insert(x);
        degree[x] ++;
        degree[y] ++;
        // for dynamic, affected
        if (node_max_id + 1 > affected_node.size()) {
            affected_node.resize(node_max_id + 1);
        }
        if (edge_number > affected_edge.size()) {
            affected_edge.resize(edge_number + 1);
        }
        affected_node[x] = true;
        affected_node[y] = true;
        affected_edge[rtn] = true;
        return rtn;
    }

    uint remove(T x, T y) {     // return edge index, for dynamic
        Edge<T> e(x, y);
        uint index = edge_index_map[e.str];
        edge_tomb[index] = true;
        adj[x].erase(y);
        adj[y].erase(x);
        degree[x] --;
        degree[y] --;
        // for dynamic affected 
        affected_node[x] = true;
        affected_node[y] = true;
        return index;
    }
    
    void endInsert() {
        edge_support.resize(edge_number);
        edge_tau.resize(edge_number);
    }

    void resetAffect() {
        fill(affected_node.begin(), affected_node.end(), false);
        fill(affected_edge.begin(), affected_edge.end(), false);
    }

    void setAffectNode(uint index) {
        affected_node[index] = true;
    }

    void setAffectEdge(uint index) {
        affected_edge[index] = true;
        affected_node[edge_list[index].a] = true;
        affected_node[edge_list[index].b] = true;
    }
};

class SuperGraph
{
public:
    vector< unordered_set<uint> > super_vertex;
    vector<uint> vertex_truss;
    vector< Edge<uint> > edge_list;
    vector< unordered_set<uint> > adj;
    vector< unordered_set<uint> > inverse_table; // node -> super node
    vector< uint > edge_inverse_table;  // edge -> super node

    vector< bool > node_tomb;   // node tomb, for dynamic update
    vector< bool > affected_super_node; // literaly

    uint super_node_number;
    uint super_edge_number;

    SuperGraph(uint nn) {
        super_node_number = 0;
        super_edge_number = 0;
        inverse_table.resize(nn + 1);
    }

    void newVertex(uint k) {
        super_vertex.push_back( unordered_set<uint>() );
        vertex_truss.push_back(k);
        super_node_number ++;
    }

    void addVertex(uint edge_no, uint u, uint v) {
        super_vertex[super_node_number - 1].insert(edge_no);
        if (edge_no + 1 > edge_inverse_table.size()) {
            edge_inverse_table.resize(edge_no + 1);
            edge_inverse_table[edge_no] = super_node_number - 1;
        }
        inverse_table[u].insert(super_node_number - 1);
        inverse_table[v].insert(super_node_number - 1);
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
        affected_super_node.resize(super_node_number + 1);
    }

    void setAffectSuperNode(uint index) {
        affected_super_node[index] = true;
        // TODO: do or not ???
        for (auto& sn : adj[index]) {
            adj[sn].erase(index);
        }
    }

    ~SuperGraph() {}
};
