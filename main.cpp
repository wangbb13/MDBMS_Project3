#include "kits.hpp"
#include "catch.hpp"
#include "edgetrussness.hpp"
#include "equitruss.hpp"
#include "search.hpp"
#include "maintenance.hpp"
#include <chrono>
using namespace std::chrono;

/* show index i.e. equitruss */
void showIndex(Graph<uint>& graph, SuperGraph& super_graph) {
    cout << "super node number = " << super_graph.super_node_number << endl; 
    cout << "super edge number = " << super_graph.super_edge_number << endl;
    cout << "super vertices : " << endl;
    vector< Edge<uint> >& e_list = graph.edge_list;
    vector< bool >& tomb = super_graph.tomb_super_node;

    for (uint i = 0; i < super_graph.super_node_number; ++ i) {
        cout << i << " : ";
        if (tomb[i]) {
            cout << " tomb " << endl;
            continue;
        }
        for (auto& eno : super_graph.super_vertex[i]) 
            cout << "(" << e_list[eno] << ") ";
        cout << endl;
    }
    cout << "super edges : " << endl;
    for (auto& item : super_graph.edge_list)
        if (!tomb[item.a] && !tomb[item.b])
            cout << item << endl;
}

void test_code()
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
    SuperGraph super_graph(graph.node_max_id);
    start = high_resolution_clock::now();
    buildEuqiTruss(graph, super_graph);
    stop = high_resolution_clock::now();
    cost = duration_cast<microseconds>(stop - start);
    cout << "[buildEuqiTruss time]: " << cost.count() << " us" << endl << endl;
    showIndex(graph, super_graph);
    cout << "eliminate affected nodes and edges" << endl;
    graph.resetAffect();    // eliminate affected nodes and edges

    // test community search
    // cout << "community search: k = 4, vertex = v4" << endl;
    // vector<uint> ans;
    // start = high_resolution_clock::now();
    // communitySearch(super_graph, ans, 4, 4);
    // stop = high_resolution_clock::now();
    // cost = duration_cast<microseconds>(stop - start);
    // cout << "[communitySearch time]: " << cost.count() << " us" << endl << endl;
    // for (auto& sv : ans)
    //     cout << sv << " ";
    // cout << endl;

    // maintenance of EquiTruss
    // vector< Edge<uint> > test_add;
    // test_add.push_back(Edge<uint>(4, 9));
    // start = high_resolution_clock::now();
    // insertBatch(graph, super_graph, test_add);
    // dynamicUpdate(graph, super_graph);
    // stop = high_resolution_clock::now();
    // cost = duration_cast<microseconds>(stop - start);
    // cout << "[re-buildEuqiTruss time]: " << cost.count() << " us" << endl << endl;
    // showIndex(graph, super_graph);
    // TODO: more test case
    vector< Edge<uint> > test_add;
    vector< Edge<uint> > test_rm;
    test_rm.push_back(Edge<uint>(6, 7));
    test_add.push_back(Edge<uint>(4, 9));
    start = high_resolution_clock::now();
    insertBatch(graph, super_graph, test_add);
    removeBatch(graph, super_graph, test_rm);
    dynamicUpdate(graph, super_graph);
    stop = high_resolution_clock::now();
    cost = duration_cast<microseconds>(stop - start);
    cout << "[re-buildEuqiTruss time]: " << cost.count() << " us" << endl << endl;
    showIndex(graph, super_graph);

    system("pause");
}

int expOne(const char* filename)
{
    Graph<uint> graph;
    uint batch = 1000;
    uint freq = 10;

    // load half amount of data
    char delimiter = '\t';
    char newline = '\n';
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << filename << " is not exists." << endl;
        return 0;
    }
    string str_line = "";
    string str_num = "";
    istringstream iss(str_line);
    uint pl, pr, fno, fnn, fen;
    // read number of nodes and edges
    getline(fin, str_line, newline);
    iss.clear();
    iss.str(str_line);
    getline(iss, str_num, delimiter);
    fnn = string2Num<uint>(str_num);
    getline(iss, str_num, delimiter);
    fen = string2Num<uint>(str_num);
    
    auto start = high_resolution_clock::now();
    uint half = fen / 2;
    for (fno = 0; fno < half; ++ fno) {
        getline(fin, str_line, newline);
        iss.clear();
        iss.str(str_line);
        getline(iss, str_num, delimiter);
        pl = string2Num<uint>(str_num);
        getline(iss, str_num, delimiter);
        pr = string2Num<uint>(str_num);
        graph.add(pl, pr);
    }
    graph.endInsert();
    auto stop = high_resolution_clock::now();
    auto cost = duration_cast<microseconds>(stop - start);
    cout << "[load half data time]: " << cost.count() << " us" << endl << endl;

    // read add or delete data
    vector< vector< Edge<uint> > > temp_edges(freq);
    for (uint i = 0; i < freq; ++ i) {
        for (uint j = 0; j < batch; ++ j) {
            getline(fin, str_line, newline);
            iss.clear();
            iss.str(str_line);
            getline(iss, str_num, delimiter);
            pl = string2Num<uint>(str_num);
            getline(iss, str_num, delimiter);
            pr = string2Num<uint>(str_num);
            temp_edges[i].push_back(Edge<uint>(pl, pr));
        }
    }

    // truss decompose
    start = high_resolution_clock::now();
    trussDecompose<uint>(graph);
    stop = high_resolution_clock::now();
    cost = duration_cast<microseconds>(stop - start);
    cout << "[trussDecompose time]: " << cost.count() << " us" << endl << endl;
    
    // build index
    SuperGraph super_graph(graph.node_max_id);
    start = high_resolution_clock::now();
    buildEuqiTruss(graph, super_graph);
    stop = high_resolution_clock::now();
    cost = duration_cast<microseconds>(stop - start);
    cout << "[buildEuqiTruss time]: " << cost.count() << " us" << endl << endl;
    graph.resetAffect();    // eliminate affected nodes and edges !!!!!

    // test insert
    uint rt = 0;
    for (uint i = 0; i < freq; ++ i) {
        insertBatch(graph, super_graph, temp_edges[i]);
        start = high_resolution_clock::now();
        dynamicUpdate(graph, super_graph);
        stop = high_resolution_clock::now();
        cost = duration_cast<microseconds>(stop - start);
        rt += cost.count();
        // cout << "insert " << i << ":" << temp_edges[i][0] << endl;
        // cout << "insert " << i << endl;
    }
    cout << "Insert Update Time : " << rt << " (" << freq << ")" << endl;

    // test delete
    rt = 0;
    for (int i = freq - 1; i >= 0; -- i) {
        removeBatch(graph, super_graph, temp_edges[i]);
        start = high_resolution_clock::now();
        dynamicUpdate(graph, super_graph);
        stop = high_resolution_clock::now();
        cost = duration_cast<microseconds>(stop - start);
        rt += cost.count();
        graph.resetAffect();    // eliminate affected nodes and edges !!!!!
        // cout << "remove " << i << endl;
    }
    cout << "Remove Update Time : " << rt << " (" << freq << ")" << endl;

    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        cout << "Usage: ./main {filename} " << endl;
        return 0;
    }
    const char* filename = argv[1];
    cout << "=============== " << filename << " ====================" << endl;
    expOne(filename);
    cout << "==================== END ==============================" << endl;
    return 0;
}