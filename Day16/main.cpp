// STL includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

int dijkstra_path(const weighted_graph &G, int s, int t, std::vector<vertex_desc> &path) {
  int n = boost::num_vertices(G);
  std::vector<int>         dist_map(n);
  std::vector<vertex_desc> pred_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G)))
    .predecessor_map(boost::make_iterator_property_map(
      pred_map.begin(), boost::get(boost::vertex_index, G))));

  int cur = t;
  path.clear(); path.push_back(cur);
  while (s != cur) {
    cur = pred_map[cur];
    path.push_back(cur);
  }
  std::reverse(path.begin(), path.end());
  return dist_map[t];
}

using namespace std;

// Function to open a file and return the input file stream
std::ifstream openFile(const std::string& fileName) {
    std::ifstream inFile(fileName);
    if (!inFile) {
        std::cerr << "Error: Could not open " << fileName << "\n";
    }
    return inFile;
}

// Function to read a file line by line and process each line
vector<string> readFileLineByLine(const std::string& fileName) {
    std::ifstream inFile = openFile(fileName);

    std::string line;
    vector<string> lines;
    while (std::getline(inFile, line)) {
        // Process the line
        lines.push_back(line);
        //std::cout << line << std::endl;
    }

    inFile.close();
    return lines;
}

// Flattening function for (i, j) position
auto get_flat_index = [&](int i, int j, int len) {
    return i * len + j;
};

// Direction vectors for neighbors (up, left, down, right)
const int DX[4] = {-1, 0, 1, 0};
const int DY[4] = {0, -1, 0, 1};

int main()
{

    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    vector<vector<bool>> nodes(lines.size(), vector<bool>(lines[0].size(), false));
    pair<int,int> start, end;
    int size = 0;
    for(int i = 0; i<lines.size(); i++){
        for(int j = 0; j<lines[0].size(); j++){
                switch (lines[i][j]) {
                    case '#': break;
                    case '.': nodes[i][j] = true; size++; break;
                    case 'S': nodes[i][j] = true; size++; start = make_pair(i,j); break;
                    case 'E': nodes[i][j] = true; size++; end = make_pair(i,j); break;
            }
        }
    }

    int grid_size = lines[0].size()*lines.size();
    weighted_graph G(grid_size*4);
    weight_map weights;
    //Ebene 0: move upwards (i*lines.size() + j)
    //Ebene 1: move right (i*lines.size() + j + grid_size)
    //Ebene 2: move downwards (i*lines.size() + j + 2*grid_size)
    //Ebene 3: move left (i*lines.size() + j + 3*grid_size)
    //Connect: 0<->1, 0<->3 | 1<->2 | 2<->3
    for(int i = 0; i<lines.size(); i++){
        for(int j = 0; j<lines[0].size(); j++){
            //0 <-> 1
            int from = i*lines.size() + j;
            int to = i*lines.size() + j + grid_size;
            edge_desc e = boost::add_edge(from, to, G).first; weights[e] = 1000;
            edge_desc e2 = boost::add_edge(to, from, G).first; weights[e2] = 1000;

            //0<->3
            to = i*lines.size() + j + 3*grid_size;
            edge_desc e3 = boost::add_edge(from, to, G).first; weights[e3] = 1000;
            edge_desc e4 = boost::add_edge(to, from, G).first; weights[e4] = 1000;

            //2<->3
            from =  i*lines.size() + j + 2*grid_size;
            edge_desc e5 = boost::add_edge(from, to, G).first; weights[e5] = 1000;
            edge_desc e6 = boost::add_edge(to, from, G).first; weights[e6] = 1000;

            //1<->2
            to = i*lines.size() + j + grid_size;
            edge_desc e7 = boost::add_edge(from, to, G).first; weights[e7] = 1000;
            edge_desc e8 = boost::add_edge(to, from, G).first; weights[e8] = 1000;
        }
    }
    //Connect edges
    for(int i = 0; i<lines.size(); i++){
        for(int j = 0; j<lines[0].size(); j++){
            if(!nodes[i][j]){
                continue;
            }
            //up (Level 0)
            if(i > 0 && nodes[i-1][j]){
                int from = i*lines.size() + j;
                int to = (i-1)*lines.size() + j;
                edge_desc e = boost::add_edge(from, to, G).first; weights[e] = 1;
            }
            //right (Level 1)
            if(j < lines[0].size()-1 && nodes[i][j+1]){
                int from = i*lines.size() + j + grid_size;
                int to = i*lines.size() + j+1 + grid_size;
                edge_desc e = boost::add_edge(from, to, G).first; weights[e] = 1;
            }
            //down (Level 2)
            if(i < lines.size()-1 && nodes[i+1][j]){
                int from = i*lines.size() + j + grid_size*2;
                int to = (i+1)*lines.size() + j + grid_size*2;
                edge_desc e = boost::add_edge(from, to, G).first; weights[e] = 1;
            }
            //left (level 3)
            if(j > 0 && nodes[i][j-1]){
                int from = i*lines.size() + j + grid_size*3;
                int to = i*lines.size() + j-1 + grid_size*3;
                edge_desc e = boost::add_edge(from, to, G).first; weights[e] = 1;
            }
        }
    }


    std::vector<std::vector<bool>> visited(lines.size(), std::vector<bool>(lines[0].size(), false));
    // A helper lambda function to mark tiles visited
    auto mark_visited = [&](const std::vector<vertex_desc>& path) {
        for (int flat_index : path) {
            // Map the flat index back to grid coordinates and level
            int level = flat_index / grid_size;
            int grid_index = flat_index % grid_size;
            int x = grid_index / lines.size();
            int y = grid_index % lines.size();
            
            // Mark the tile visited if it belongs to any shortest path
            visited[x][y] = true;
        }
    };

    int start_node = start.first*lines.size() + start.second + grid_size;
    cout << start_node << "\n";
    std::vector<int> end_nodes = {
        end.first * (int)lines.size() + end.second,             // North (level 0)
        end.first * (int)lines.size() + end.second + grid_size, // East (level 1)
        end.first * (int)lines.size() + end.second + 2 * grid_size, // South (level 2)
        end.first * (int)lines.size() + end.second + 3 * grid_size  // West (level 3)
    };
    int shortest_distance = std::numeric_limits<int>::max();
    for (int end_node : end_nodes) {
        cout << end_node << " ";
        int curr = dijkstra_dist(G, start_node, end_node);
        if (curr < shortest_distance) {
            shortest_distance = curr;
        }
    }
    
    std::cout <<"\n" <<  shortest_distance << "\n";


  return 0;
}