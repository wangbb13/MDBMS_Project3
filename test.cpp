#define CATCH_CONFIG_MAIN
#include "kits.hpp"
#include "catch.hpp"
#include "edgetrussness.hpp"
#include "equitruss.hpp"

TEST_CASE ("test read data", "kits.hpp readData") {
    Graph<uint> graph;
    readData<uint>("./TEST/ungraph.txt", graph, 0, ' ');
    cout << "number of lines = " << graph.edge_number << endl;
    cout << "min node id = " << graph.node_min_id << ", max node id = " << graph.node_max_id << endl;
    cout << "first 5 lines: " << endl;
    for (uint _ = 0; _ < 5; ++ _) 
        cout << graph.edge_list[_] << endl;
    trussDecompose<uint>(graph);
    uint en = graph.edge_number;
    for (uint i = 0; i < en; ++ i) 
        cout << graph.edge_list[i] << " : " << graph.edge_tau[i] << endl;
    cout << "min edge trussness = " << graph.min_edge_tau << endl;
    cout << "max edge trussness = " << graph.max_edge_tau << endl;

    SuperGraph super_graph;
    buildEuqiTruss(graph, super_graph);
    cout << "node number = " << super_graph.super_node_number << endl; 
    cout << "edge number = " << super_graph.super_edge_number << endl;
    cout << "vertices : " << endl;
    vector< Edge<uint> >& e_list = graph.edge_list;
    for (uint i = 0; i < super_graph.super_node_number; ++ i) {
        for (auto& eno : super_graph.super_vertex[i]) 
            cout << "(" << e_list[eno] << ") ";
        cout << endl;
    }

    cout << "edges : " << endl;
    for (auto& item : super_graph.edge_list)
        cout << item << endl;
}
