#pragma once
#include "kits.hpp"
using namespace std;

/*
 * Input  > edges: graph.edge_list
 * Output < res: graph.edge_support
 */
template <typename T>
void edgeSup(Graph<T>& graph) {
    uint u, nmi = graph.node_min_id, nmx = graph.node_max_id, nn = graph.edge_number;
    vector<uint>& sup = graph.edge_support;
    vector< unordered_set<T> >& adj = graph.adj;
    unordered_map<string, uint>& func = graph.edge_index_map;
    vector<bool> visit(nn+1, true);
    string e1_str = "", e2_str = "", e3_str = "";
    for (u = nmi; u < nmx-1; ++ u) {
        for (auto& v : adj[u]) {
            e1_str = edgeStr<T>(u, v);
            if (visit[func[e1_str]]) {
                for (auto& w : adj[v]) {
                    e2_str = edgeStr<T>(u, w);
                    if (func.find(e2_str) != func.end() &&
                        visit[func[e2_str]]) {
                        e3_str = edgeStr<T>(v, w);
                        sup[func[e1_str]] ++;
                        sup[func[e2_str]] ++;
                        sup[func[e3_str]] ++;
                    }
                }
            }
            visit[func[e1_str]] = false;
        }
    }
}

/*
 * Input  > edges: graph.edge_list
 * Output < res: graph.edge_tau
 */
template <typename T>
void trussDecompose(Graph<T>& graph) {
    edgeSup(graph);
    uint k = 2, en = graph.edge_number, hasv = 0;
    uint i, j, t, msv, msi, index;
    T u, v, a, b;
    string e1_str =  "", e2_str = "";

    vector< unordered_set<T> > adj(graph.adj);
    vector<uint> sup(graph.edge_support);
    vector<bool> visit(en, true);
    vector<uint>& tau = graph.edge_tau;
    unordered_map<string, uint>& func = graph.edge_index_map;
    set< pair< uint, uint > > bst;  // support value, edge index

    for (i = 0; i < en; ++ i) 
        bst.insert(make_pair(sup[i], i));
    
    msv = bst.begin()->first;
    msi = bst.begin()->second;
    k = msv + 2;
    graph.min_edge_tau = k;

    while (hasv < en) {
        while (hasv < en && msi >= 0 && msv <= (k-2)) {
            a = graph.edge_list[msi].a;
            b = graph.edge_list[msi].b;
            if (adj[a].size() <= adj[b].size()) {
                u = a; v = b;
            } else {
                u = b; v = a;
            }
            for (auto& w : adj[u]) {
                e2_str = edgeStr<T>(v, w);
                if (func.find(e2_str) != func.end() &&
                    visit[func[e2_str]]) {
                    e1_str = edgeStr<T>(u, w);

                    index = func[e1_str];
                    bst.erase(make_pair(sup[index], index));
                    sup[index] --;
                    bst.insert(make_pair(sup[index], index));
                    index = func[e2_str];
                    bst.erase(make_pair(sup[index], index));
                    sup[index] --;
                    bst.insert(make_pair(sup[index], index));
                }
            }
            tau[msi] = k;
            hasv ++;
            visit[msi] = false;
            adj[u].erase(v);
            adj[v].erase(u);
            // min in bst
            bst.erase(make_pair(msv, msi));
            msv = bst.begin()->first;
            msi = bst.begin()->second;
        }
        k ++;
    }
    graph.max_edge_tau = k - 1;
}
