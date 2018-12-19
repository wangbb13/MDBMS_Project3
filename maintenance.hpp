#pragma once
#include "kits.hpp"

/*
 * Input  < affected node index, affected super node
 *          graph, super_graph
 * Output > updated super_graph, i.e. new index equitruss
 */
void dynamicUpdate() {

}

/*
 * literaly
 */
void insertBatch(Graph<uint>& graph, 
    SuperGraph& super_graph, 
    vector< Edge<uint> >& edges) {
    vector< unordered_set<uint> >& adj = graph.adj;
    unordered_map<string, uint>& func = graph.edge_index_map;
    vector<uint>& tau = graph.edge_tau;
    vector<uint>& e_inverse = super_graph.edge_inverse_table;
    vector< unordered_set<uint> >& super_n = super_graph.super_vertex;
    
    vector<uint> common;
    string e1_str = "", e2_str = "";
    uint max_id = graph.node_max_id;
    uint u, v, k, sn, eno, mark = 0, i;
    for (auto& item : edges) {
        u = item.a;
        v = item.b;
        graph.add(u, v);
        if (u <= max_id && v <= max_id) {   // add affected nodes and edges
            k = 2;
            mark = 0;
            for (auto& w : adj[u]) {
                if (adj[v].find(w) != adj[v].end()) {
                    k ++;
                    if (mark + 1 < common.size()) {
                        common.push_back(w);
                        mark ++;
                    }
                    else
                        common[mark ++] = w;
                }
            }
            for (i = 0; i < mark; ++ i) {
                Edge<uint> euw(u, common[i]);
                Edge<uint> evw(v, common[i]);
                e1_str = euw.str;
                e2_str = evw.str;
                eno = func[e1_str];
                if (tau[eno] < k) {
                    sn = e_inverse[eno];
                    super_graph.setAffectSuperNode(sn);
                    for (auto& _e_ : super_n[sn]) {
                        graph.setAffectEdge(_e_);
                    }
                }
                eno = func[e2_str];
                if (tau[eno] < k) {
                    sn = e_inverse[eno];
                    super_graph.setAffectSuperNode(sn);
                    for (auto& _e_ : super_n[sn]) {
                        graph.setAffectEdge(_e_);
                    }
                }
            }
        }
    }
}