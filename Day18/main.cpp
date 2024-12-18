// STL includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <set>

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

int main()
{

    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    set<pair<int, int>> coordinates;
    vector<pair<int,int>> input;

    int y_size = 71; int x_size = 71;
    int low = 0, high = 3449, result_k = -1;


    for (const string& line : lines) {
        stringstream ss(line);
        string xStr, yStr;
        getline(ss, xStr, ',');
        getline(ss, yStr, ',');
        int x = stoi(xStr);
        int y = stoi(yStr);
        input.push_back(make_pair(x, y));
    }

    //binary search for efficiency
    while (low <= high) {
        int mid = (low + high) / 2;

        set<pair<int, int>> coordinates;
        for (int l = 0; l < mid; l++) {
            coordinates.insert(input[l]);
        }

        // Build the graph
        weighted_graph G(x_size * y_size);
        weight_map weights;

        for (int y = 0; y < y_size; y++) {
            for (int x = 0; x < x_size; x++) {
                int vertex_id = y * x_size + x;
                if (coordinates.find({x, y}) != coordinates.end()) {
                    continue;
                }

                // Vertical edges
                if (y > 0 && coordinates.find({x, y - 1}) == coordinates.end()) {
                    edge_desc e = add_edge(vertex_id - x_size, vertex_id, G).first;
                    weights[e] = 1;
                    edge_desc e2 = add_edge(vertex_id, vertex_id - x_size, G).first;
                    weights[e2] = 1;
                }

                // Horizontal edges
                if (x > 0 && coordinates.find({x - 1, y}) == coordinates.end()) {
                    edge_desc e = add_edge(vertex_id - 1, vertex_id, G).first;
                    weights[e] = 1;
                    edge_desc e2 = add_edge(vertex_id, vertex_id - 1, G).first;
                    weights[e2] = 1;
                }
            }
        }

        int dist = dijkstra_dist(G, 0, (y_size - 1) * x_size + (x_size - 1));
        std::cout << "k: " << mid << ", dist: " << dist << ", coordinates size: " << coordinates.size() << "\n";

        if (dist > 1000000) {
            result_k = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    std::cout << "Result: " << input[result_k-1].first << "," << input[result_k-1].second << "\n";
    


    inFile.close();

    return 0;
}
