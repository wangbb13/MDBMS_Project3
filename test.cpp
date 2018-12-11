#define CATCH_CONFIG_MAIN
#include "kits.hpp"
#include "catch.hpp"

// TEST_CASE ("test read data", "kits.hpp readData") {
//     vector< pair<uint, uint> > vec;
//     uint node_min, node_max;
//     readData<uint>("./DBLP/dblp.ungraph.txt", vec, node_min, node_max);
//     cout << "number of lines = " << vec.size() << endl;
//     cout << "min node id = " << node_min << ", max node id = " << node_max << endl;
//     cout << "first 5 lines: " << endl;
//     for (uint _ = 0; _ < 5; ++ _) 
//         cout << vec[_].first <<  " " << vec[_].second << endl;
// }

TEST_CASE ("test unordered_map", " ") {
    unordered_map< pair<uint, uint>, uint > pu_map;
    pair<uint, uint> a = { 0, 1 };
    pu_map.insert(make_pair(a, 0));
}
