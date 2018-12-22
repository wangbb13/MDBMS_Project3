#pragma once
#include "kits.hpp"
using namespace std;

/*
 * Input  > edges: graph.edge_list
 * Output < res: graph.edge_support
 */
template <typename T>
void edgeSup(Graph<T>& graph) {
    uint nmi = graph.node_min_id, nmx = graph.node_max_id, nn = graph.edge_number;
    uint u, eno1;

    vector<uint>& sup = graph.edge_support;
    vector< unordered_set<T> >& adj = graph.adj;
    unordered_map<string, uint>& func = graph.edge_index_map;
    vector< bool >& aft_node = graph.affected_node;
    // vector< bool >& aft_edge = graph.affected_edge;

    vector<bool> visit(nn+1, true);
    string e1_str = "", e2_str = "", e3_str = "";

    for (u = nmi; u < nmx-1; ++ u) {
        if (!aft_node[u])
            continue;
        for (auto& v : adj[u]) {
            if (aft_node[v]) {
                e1_str = edgeStr<T>(u, v);
                eno1 = func[e1_str];
                if (visit[eno1]) {
                    for (auto& w : adj[v]) {
                        e2_str = edgeStr<T>(u, w);
                        if (adj[u].find(w) != adj[u].end() &&
                            visit[func[e2_str]]) {
                            e3_str = edgeStr<T>(v, w);
                            sup[eno1] ++;
                            if (aft_node[w]) {
                                sup[func[e2_str]] ++;
                                sup[func[e3_str]] ++;
                            }
                        }
                    }
                }
                visit[eno1] = false;
            }
        }
    }

    #ifdef ALGSUP
    cout << endl;
    vector< Edge<uint> >& e_list = graph.edge_list;
    // for (int i = 0; i < e_list.size(); ++ i) 
    //     cout << e_list[i] << "(" << sup[i] << ") " << endl;
    cout << "calc support end" << endl;
    #endif
}

/*
 * Input  > edges: graph.edge_list
 *          Just affected edges
 * Output < res: graph.edge_tau
 */
template <typename T>
void trussDecompose(Graph<T>& graph) {
    edgeSup(graph);
    uint k = 2, en = graph.edge_number, hasv = 0, afn = 0;
    uint i, j, t, msv = 0x5fffffff, msi = -1, index;
    T u, v, a, b;
    string e1_str =  "", e2_str = "";

    vector< unordered_set<T> > adj(graph.adj);
    vector<uint> sup(graph.edge_support);
    vector<bool> visit(en, true);
    vector<uint>& tau = graph.edge_tau;
    unordered_map<string, uint>& func = graph.edge_index_map;
    vector<bool>& aft_edge = graph.affected_edge;
    set< pair< uint, uint > > bst;  // support value, edge index

    for (i = 0; i < en; ++ i) {
        if (aft_edge[i]) {
            bst.insert(make_pair(sup[i], i));
            afn ++;
        }
    }
    msv = bst.begin()->first;
    msi = bst.begin()->second;
    k = msv + 2;
    graph.min_edge_tau = min(graph.min_edge_tau, k);

    #ifdef ALGDECOMP
    cout << "edge number = " << en << ", afn = " << afn << endl;
    cout << "k = " << k << endl;
    #endif

    while (hasv < afn) {
        while (hasv < afn && msi >= 0 && msv <= (k-2)) {
            a = graph.edge_list[msi].a;
            b = graph.edge_list[msi].b;
            if (adj[a].size() <= adj[b].size()) {
                u = a; v = b;
            } else {
                u = b; v = a;
            }
            for (auto& w : adj[u]) {
                e2_str = edgeStr<T>(v, w);
                if (adj[v].find(w) != adj[v].end() &&
                    visit[func[e2_str]] && 
                    aft_edge[func[e2_str]]) {
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
    graph.max_edge_tau = max(graph.max_edge_tau, k - 1);

    #ifdef ALGDECOMP
    cout << "min tau = " << graph.min_edge_tau << endl;
    cout << "max tau = " << graph.max_edge_tau << endl;

    // for (i = 0; i < en; ++ i) 
    //     if (!graph.edge_tomb[i])
    //         cout << graph.edge_list[i] << " : " << graph.edge_tau[i] << endl;
    #endif
}
