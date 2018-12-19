#include "kits.hpp"
#include "catch.hpp"
#include "edgetrussness.hpp"
#include "equitruss.hpp"
#include "search.hpp"
#include <chrono>
using namespace std::chrono;

int main(int argc, char const *argv[])
{
    Graph<uint> graph;

    // test read data
    auto start = high_resolution_clock::now();
    readData<uint>("./TEST/ungraph.txt", graph, 0, ' ');
    auto stop = high_resolution_clock::now();
    auto cost = duration_cast<microseconds>(stop - start);
    cout << "[load data time]: " << cost.count() << " us" << endl << endl;
    cout << "number of lines = " << graph.edge_number << endl;
    cout << "min node id = " << graph.node_min_id << ", max node id = " << graph.node_max_id << endl;
    cout << "first 1 lines: " << endl;
    for (uint _ = 0; _ < 1; ++ _) 
        cout << graph.edge_list[_] << endl;

    // test calculate edge trussness
    start = high_resolution_clock::now();
    trussDecompose<uint>(graph);
    stop = high_resolution_clock::now();
    cost = duration_cast<microseconds>(stop - start);
    cout << "[trussDecompose time]: " << cost.count() << " us" << endl << endl;
    uint en = graph.edge_number;
    for (uint i = 0; i < en; ++ i) 
        cout << graph.edge_list[i] << " : " << graph.edge_tau[i] << endl;
    cout << "min edge trussness = " << graph.min_edge_tau << endl;
    cout << "max edge trussness = " << graph.max_edge_tau << endl;

    // test index EquiTruss
    SuperGraph super_graph(graph.node_number);
    start = high_resolution_clock::now();
    buildEuqiTruss(graph, super_graph);
    stop = high_resolution_clock::now();
    cost = duration_cast<microseconds>(stop - start);
    cout << "[buildEuqiTruss time]: " << cost.count() << " us" << endl << endl;
    cout << "super node number = " << super_graph.super_node_number << endl; 
    cout << "super edge number = " << super_graph.super_edge_number << endl;
    cout << "super vertices : " << endl;
    vector< Edge<uint> >& e_list = graph.edge_list;
    for (uint i = 0; i < super_graph.super_node_number; ++ i) {
        cout << i << " : ";
        for (auto& eno : super_graph.super_vertex[i]) 
            cout << "(" << e_list[eno] << ") ";
        cout << endl;
    }
    cout << "super edges : " << endl;
    for (auto& item : super_graph.edge_list)
        cout << item << endl;

    // test community search
    cout << "community search: k = 4, vertex = v4" << endl;
    vector<uint> ans;
    start = high_resolution_clock::now();
    communitySearch(super_graph, ans, 4, 4);
    stop = high_resolution_clock::now();
    cost = duration_cast<microseconds>(stop - start);
    cout << "[communitySearch time]: " << cost.count() << " us" << endl << endl;
    for (auto& sv : ans)
        cout << sv << " ";
    cout << endl;

    // maintenance of EquiTruss
    // TODO

    system("pause");
    return 0;
}