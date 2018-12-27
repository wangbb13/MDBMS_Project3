#include "kits.hpp"
#include "catch.hpp"
#include "edgetrussness.hpp"
#include "equitruss.hpp"
#include "search.hpp"
#include <ctime>
#include <chrono>
using namespace std::chrono;

uint randint(uint l, uint r) { // [l, r)
    return (uint)(rand() % (r - l) + l);
}

void expBuildIndex(const char* filename) {
    Graph<uint> graph;

    // load data
    auto start = high_resolution_clock::now();
    readData<uint>(filename, graph);
    auto stop = high_resolution_clock::now();
    auto cost = duration_cast<microseconds>(stop - start);
    cout << "[load data time]: " << cost.count() << " us" << endl << endl;
    cout << "number of lines = " << graph.edge_number << endl;
    cout << "min node id = " << graph.node_min_id << ", max node id = " << graph.node_max_id << endl;

    // calc edge trussness
    SuperGraph super_graph(graph.node_number);
    start = high_resolution_clock::now();
    trussDecompose<uint>(graph);
    stop = high_resolution_clock::now();
    cost = duration_cast<microseconds>(stop - start);
    cout << "[edge trussness time]: " << cost.count() << " us" << endl << endl;
    cout << "min edge trussness = " << graph.min_edge_tau << endl;
    cout << "max edge trussness = " << graph.max_edge_tau << endl;
    
    // build index, i.e. equitruss
    start = high_resolution_clock::now();
    buildEuqiTruss(graph, super_graph);
    stop = high_resolution_clock::now();
    cost = duration_cast<microseconds>(stop - start);
    cout << "[buildEuqiTruss time]: " << cost.count() << " us" << endl << endl;
    cout << "index size = " << super_graph.space() << endl;
    cout << "super node number = " << super_graph.super_node_number << endl;
    cout << "super edge number = " << super_graph.super_edge_number << endl;

    uint exp_times = 100;
    uint gnn = graph.node_number;
    vector<uint> ans;

    // exp 2, degree rank 
    vector< pair<uint, uint> > degree_id(gnn);
    for (uint i = 0; i < gnn; ++ i) 
        degree_id[i] = make_pair(graph.degree[i], i);
    sort(degree_id.begin(), degree_id.end());
    uint step = (uint)(gnn / 10);
    for (uint i = 0; i < 10; ++ i) {
        uint sl = (uint)(step * i);
        uint end = sl + step;
        for (uint kt = 4; kt < 7; kt ++) {
            uint rt = 0;
            for (uint j = 0; j < exp_times; ++ j) {
                uint q = degree_id[randint(sl, end)].second;
                start = high_resolution_clock::now();
                communitySearch(super_graph, ans, kt, q);
                stop = high_resolution_clock::now();
                cost = duration_cast<microseconds>(stop - start);
                rt += cost.count();
                ans.clear();
                // cout << q << " : " << ans.size() << endl;
            }
            cout << "Degree Rank : " << i << " Truss " << kt << " searching time : " << rt << endl;
        }
    }
    
    // exp 3, edge truss : 3, 4, 5, 6, 7
    cout << endl;
    uint p30 = (uint)(0.3 * gnn);
    uint p70 = (uint)(0.7 * gnn);
    for (uint kt = 3; kt < 8; ++ kt) {
        uint rt = 0;
        for (uint j = 0; j < exp_times / 2; ++ j) {
            uint q = degree_id[randint(0, p30)].second;
            start = high_resolution_clock::now();
            communitySearch(super_graph, ans, kt, q);
            stop = high_resolution_clock::now();
            cost = duration_cast<microseconds>(stop - start);
            rt += cost.count();
            ans.clear();
            // cout << q << " : " << ans.size() << endl;
        }
        for (uint j = exp_times / 2; j < exp_times; ++ j) {
            uint q = degree_id[randint(p70, gnn)].second;
            start = high_resolution_clock::now();
            communitySearch(super_graph, ans, kt, q);
            stop = high_resolution_clock::now();
            cost = duration_cast<microseconds>(stop - start);
            rt += cost.count();
            ans.clear();
            // cout << q << " : " << ans.size() << endl;
        }
        cout << "Truss : " << kt << " searching time : " << rt << endl;
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        cout << "Usage: ./main {filename} " << endl;
        return 0;
    }
    srand((unsigned)time(NULL));
    const char* filename = argv[1];
    cout << "=============== " << filename << " ====================" << endl;
    expBuildIndex(filename);
    cout << "==================== END ==============================" << endl;
    return 0;
}