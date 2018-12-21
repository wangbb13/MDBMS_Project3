#pragma once
#include "kits.hpp"
#include "edgetrussness.hpp"
#include "equitruss.hpp"

/*
 * Input  < affected node index, affected super node
 *          graph, super_graph
 * Output > updated super_graph, i.e. new index equitruss
 */
void dynamicUpdate(Graph<uint>& graph, 
    SuperGraph& super_graph) {
    vector< unordered_set<uint> >& s_adj = super_graph.adj;
    vector< bool >& affect_sn = super_graph.affected_super_node;
    vector< bool >& tomb_sn = super_graph.tomb_super_node;
    vector< uint >& sp_truss = super_graph.vertex_truss;
    vector< unordered_set<uint> >& super_n = super_graph.super_vertex;
    uint i, j, u, v;

    // remove super edges
    for (i = 0; i < super_graph.super_node_number; ++ i) {
        if (super_graph.affected_super_node[i]) {
            for (auto& sn : s_adj[i]) 
                s_adj[sn].erase(i);
        }
    }

    // re-calc edge trussness
    trussDecompose<uint>(graph);
    // re-build equitruss
    buildEuqiTruss<uint>(graph, super_graph);

    // merge
    uint sn_number = super_graph.super_node_number;
    uint start = super_graph.marked_position;
    // affected super node -> tomb node
    for (i = 0; i < start; ++ i) 
        if (affect_sn[i])
            tomb_sn[i] = true;

    for (i = start; i < sn_number; ++ i) {
        for (auto& w : s_adj[i]) {
            if ((i > w) && sp_truss[i] == sp_truss[w]) {
                if (super_n[i].size() < super_n[w].size()) {
                    u = i; v = w;
                } else {
                    u = w; v = i;
                }
                // merge : extend node
                for (auto& elem : super_n[u])
                    super_n[v].insert(elem);
                // merge : re-link super edge
                for (auto& elem : s_adj[u]) {
                    s_adj[v].insert(elem);
                    s_adj[elem].insert(v);
                    s_adj[elem].erase(u);
                }
                // tomb u
                tomb_sn[u] = true;
            }
        }
    }

    graph.resetAffect();    // eliminate affected nodes and edges
}

/*
 * literaly
 */
void insertBatch(Graph<uint>& graph, 
    SuperGraph& super_graph, 
    vector< Edge<uint> >& edges) {
    vector< Edge<uint> >& e_list = graph.edge_list;
    vector< unordered_set<uint> >& adj = graph.adj;
    unordered_map<string, uint>& func = graph.edge_index_map;
    vector<uint>& tau = graph.edge_tau;
    vector<uint>& sup = graph.edge_support;
    vector<uint>& e_inverse = super_graph.edge_inverse_table;
    vector< unordered_set<uint> >& super_n = super_graph.super_vertex;
    
    vector<uint> common;
    unordered_set<uint> exists;
    string e1_str = "", e2_str = "";
    uint max_id = graph.node_max_id;
    uint u, v, k, sn, eno, mark = 0, i, j;
    for (auto& item : edges) {
        u = item.a;
        v = item.b;
        graph.add(u, v);
        if (u <= max_id)
            exists.insert(u);
        if (v <= max_id)
            exists.insert(v);
        if (u <= max_id && v <= max_id) {   // add affected nodes and edges
            k = 2;
            mark = 0;
            for (auto& w : adj[u]) {
                if (adj[v].find(w) != adj[v].end()) {
                    k ++;
                    if (mark + 1 > common.size()) {
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
                        if (e_list[_e_].a <= max_id)
                            exists.insert(e_list[_e_].a);
                        if (e_list[_e_].b <= max_id)
                            exists.insert(e_list[_e_].b);
                    }
                }
                eno = func[e2_str];
                if (tau[eno] < k) {
                    sn = e_inverse[eno];
                    super_graph.setAffectSuperNode(sn);
                    for (auto& _e_ : super_n[sn]) {
                        graph.setAffectEdge(_e_);
                        if (e_list[_e_].a <= max_id)
                            exists.insert(e_list[_e_].a);
                        if (e_list[_e_].b <= max_id)
                            exists.insert(e_list[_e_].b);
                    }
                }
            }
        }
    }

    vector<uint> ext_vec(exists.size());
    i = 0;
    for (auto& elem : exists) 
        ext_vec[i ++] = elem;
    
    #ifdef INSERT
    cout << "reset edge support" << endl;
    #endif
    for (i = 0; i < ext_vec.size(); ++ i) {
        for (j = i+1; j < ext_vec.size(); ++ j) {
            string str = edgeStr<uint>(ext_vec[i], ext_vec[j]);
            if (func.find(str) != func.end()) {
                #ifdef INSERT
                cout << str << endl;
                #endif
                sup[func[str]] = 0;
            }
        }
    }
    graph.endInsert();
    super_graph.resetNodeNumber(graph.node_max_id);
}

void removeBatch(Graph<uint>& graph, 
    SuperGraph& super_graph, 
    vector< Edge<uint> >& edges) {
    vector< Edge<uint> >& e_list = graph.edge_list;
    vector< unordered_set<uint> >& adj = graph.adj;
    vector< unordered_set<uint> >& s_adj = super_graph.adj;
    unordered_map<string, uint>& func = graph.edge_index_map;
    vector<uint>& tau = graph.edge_tau;
    vector<uint>& sup = graph.edge_support;
    vector<uint>& e_inverse = super_graph.edge_inverse_table;
    vector< unordered_set<uint> >& super_n = super_graph.super_vertex;
    uint eno, esn, u, v, sn, i, j;
    uint etruss;
    uint sn_number = super_graph.super_node_number;
    uint max_id = graph.node_max_id;
    vector< bool > flag(sn_number + 1, true);

    unordered_set<uint> exists;
    for (auto& item : edges) {
        graph.remove(item);
        eno = func[item.str];
        etruss = tau[eno];
        sn = e_inverse[eno];
        super_n[sn].erase(eno);
        flag[sn] = false;
        // belong to sn
        super_graph.setAffectSuperNode(sn);  // super graph
        #ifdef REMOVE
        cout << "affect super node : " << sn << endl;
        #endif
        for (auto& _e_ : super_n[sn]) {
            graph.setAffectEdge(_e_);
            if (e_list[_e_].a <= max_id)
                exists.insert(e_list[_e_].a);
            if (e_list[_e_].b <= max_id)
                exists.insert(e_list[_e_].b);
        }
        // neighbors
        u = item.a;
        v = item.b;
        for (auto& w : adj[u]) {
            if (adj[v].find(w) != adj[v].end()) {
                Edge<uint> euw(u, w);
                Edge<uint> evw(v, w);
                eno = func[euw.str];
                esn = e_inverse[eno];
                if (flag[esn] && tau[eno] < etruss && s_adj[sn].find(esn) != s_adj[sn].end()) { // affected neighbor super node 
                    super_graph.setAffectSuperNode(esn); // super graph
                    for (auto& _e_ : super_n[esn]) {       // graph
                        graph.setAffectEdge(_e_);
                        if (e_list[_e_].a <= max_id)
                            exists.insert(e_list[_e_].a);
                        if (e_list[_e_].b <= max_id)
                            exists.insert(e_list[_e_].b);
                    }
                }
                eno = func[evw.str];
                esn = e_inverse[eno];
                if (flag[esn] && tau[eno] < etruss && s_adj[sn].find(esn) != s_adj[sn].end()) { // affected neighbor super node 
                    super_graph.setAffectSuperNode(esn); // super graph
                    for (auto& _e_ : super_n[esn]) {     // graph
                        graph.setAffectEdge(_e_);
                        if (e_list[_e_].a <= max_id)
                            exists.insert(e_list[_e_].a);
                        if (e_list[_e_].b <= max_id)
                            exists.insert(e_list[_e_].b);
                    }
                }
            }
        }
    }

    vector<uint> ext_vec(exists.size());
    i = 0;
    for (auto& elem : exists)
        ext_vec[i ++] = elem;
    for (i = 0; i < ext_vec.size(); ++ i) {
        for (j = i+1; j < ext_vec.size(); ++ j) {
            string str = edgeStr<uint>(ext_vec[i], ext_vec[j]);
            if (func.find(str) != func.end())
                sup[func[str]] = 0;
        }
    }
}