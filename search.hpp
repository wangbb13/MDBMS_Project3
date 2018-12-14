#pragma once
#include "kits.hpp"
#include <queue>
using namespace std;

/*
 * Input  < Super Graph, i.e. EquiTruss index
 *          k, truss value
 *          q, query vertex 
 * Output > Set of super node index
 */
void communitySearch(SuperGraph& super_graph, 
    vector<uint>& ans, uint k, uint q) {
    vector< unordered_set<uint> >& inv_tbl = super_graph.inverse_table;
    vector< unordered_set<uint> >& adj = super_graph.adj;
    vector<uint>& sv_truss = super_graph.vertex_truss;
    uint vn = super_graph.super_node_number;
    vector<bool> visit(vn + 1, true);
    uint svno;

    queue<uint> my_q;
    for (auto& i : inv_tbl[q]) {
        if (visit[i] && sv_truss[i] >= k) {
            my_q.push(i);
            while (!my_q.empty()) {
                svno = my_q.front();
                my_q.pop();
                visit[svno] = false;
                ans.push_back(svno);
                for (auto& nb : adj[svno]) 
                    if (visit[nb] && sv_truss[nb] >= k)
                        my_q.push(nb);
            }
        }
    }
}