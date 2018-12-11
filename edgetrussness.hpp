#pragma once
#include "kits.hpp"

/*
 * Input  > edges: edge list
 * Output < res: support of each edge
 */
template <typename T>
void edgeSup(vector< pair<T, T> >& edges, 
             vector<uint>& res, 
             T nodes) {
    vector<uint> degree(nodes+1, 0);
    vector< vector<T> > adj(nodes+1);
    uint edges_num = edges.size();
    res.resize(edges_num);
    for (auto& _ : edges) {
        degree[_.first] ++;
        degree[_.second] ++;
        adj[_.first].push_back(_.second);
        adj[_.second].push_back(_.first);
    }
    sort();
}

/*
 * Input  > edges: edge list
 * Output < res: trussness of each edge
 */
template <typename T>
void trussDecompose(vector< pair<T, T> >& edges, 
                    vector<uint>& res) {
    return;
}
