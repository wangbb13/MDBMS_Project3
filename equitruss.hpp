#pragma once
#include "kits.hpp"
#include <queue>
using namespace std;

/*
 * Input  < graph with edge trussness !!!
 * Output > super graph, i.e. index EquiTruss
 */
template <typename T>
void buildEuqiTruss(Graph<T>& graph, SuperGraph& super_graph) {
    uint en = graph.edge_number;
    uint min_k = graph.min_edge_tau;
    uint max_k = graph.max_edge_tau;
    uint range = max_k - min_k + 1;
    uint i, j, k, s, e, snID, eno1, eno2;
    string e1_str = "", e2_str = "";
    T a, b, u, v;

    vector< Edge<T> >& e_list = graph.edge_list;
    unordered_map<string, uint>& func = graph.edge_index_map;
    vector< unordered_set<T> > adj(graph.adj);
    vector<uint>& truss = graph.edge_tau;
    vector<bool> visit(en + 1, true);
    vector<bool> level(en + 1, true);
    vector< vector<uint> > phi(range);
    vector< unordered_set<uint> > list(en + 1);

    for (i = 0; i < en; ++ i) 
        phi[truss[i] - min_k].push_back(i);

    queue<uint> my_q;
    unordered_set<uint> temp_set;
    snID = 0;
    for (k = min_k; k <= max_k; ++ k) {
        s = phi[k - min_k].size();
        for (auto& j : phi[k - min_k]) {
            if (level[j]) {
                visit[j] = false;
                super_graph.newVertex(truss[j]);
                my_q.push(j);
                while (!my_q.empty()) {
                    e = my_q.front();
                    my_q.pop();
                    a = e_list[e].a;
                    b = e_list[e].b;
                    super_graph.addVertex(e, a, b);
                    temp_set.insert(list[e].begin(), list[e].end());
                    if (adj[a].size() < adj[b].size()) {
                        u = a; v = b;
                    } else {
                        u = b; v = a;
                    }
                    for (auto& w : adj[u]) {
                        if (adj[v].find(w) != adj[v].end()) {
                            e1_str = edgeStr(u, w);
                            eno1 = func[e1_str];
                            if (truss[eno1] < k)
                                continue;
                            e2_str = edgeStr(v, w);
                            eno2 = func[e2_str];
                            if (truss[eno2] < k)
                                continue;
                            if (truss[eno1] == k) {
                                if (visit[eno1]) {
                                    visit[eno1] = false;
                                    my_q.push(eno1);
                                }
                            } else {
                                if (list[eno1].find(snID) == list[eno1].end()) {
                                    list[eno1].insert(snID);
                                }
                            }
                            if (truss[eno2] == k) {
                                if (visit[eno2]) {
                                    visit[eno2] = false;
                                    my_q.push(eno2);
                                }
                            } else {
                                if (list[eno2].find(snID) == list[eno2].end()) {
                                    list[eno2].insert(snID);
                                }
                            }
                        }
                    }
                    level[e] = false;
                    adj[a].erase(b);
                    adj[b].erase(a);
                }
                if (temp_set.size() > 0) {
                    for (auto& id : temp_set)
                        super_graph.newEdge(id, snID);
                    temp_set.clear();
                }
                snID ++;
            }
        }
    }
    super_graph.end();
}
