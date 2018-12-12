#define CATCH_CONFIG_MAIN
#include "kits.hpp"
#include "catch.hpp"
#include "edgetrussness.hpp"

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
}
