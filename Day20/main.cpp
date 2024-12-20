// STL includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <set>
#include <cmath>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

std::vector<int> dijkstra_dist(const weighted_graph &G, int s) {
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);

    boost::dijkstra_shortest_paths(G, s,
        boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));

    return dist_map;
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

//both parts done. Note that dijkstra was only used as it was useful in earlier problems and i copied it. The problem description would allow for BFS or DFS

int main()
{

    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    int size_x = lines[0].size();
    int size_y = lines.size();

    vector<vector<bool>> nodes(size_x, vector<bool>(size_y, false));
    pair<int,int> start, end;
    int size = 0;
    for(int x = 0; x<size_x; x++){
        for(int y = 0; y<size_y; y++){
                switch (lines[y][x]) {
                    case '#': break;
                    case '.': nodes[x][y] = true; size++; break;
                    case 'S': nodes[x][y] = true; size++; start = make_pair(x,y); break;
                    case 'E': nodes[x][y] = true; size++; end = make_pair(x,y); break;
            }
        }
    }


    weighted_graph G(size_x * size_y);
    weight_map weights;

    for(int x = 1; x<size_x-1; x++){
        for(int y = 1; y<size_y-1; y++){
            if(!nodes[x][y]){
                continue;
            }
            if(nodes[x+1][y]){
                int from = x*size_y + y;
                int to = (x+1)*size_y + y;
                edge_desc e = boost::add_edge(from, to, G).first; weights[e] = 1;
                edge_desc e2 = boost::add_edge(to, from, G).first; weights[e2] = 1;
            }
            if(nodes[x][y+1]){
                int from = x*size_y + y;
                int to = x*size_y + y+1;
                edge_desc e = boost::add_edge(from, to, G).first; weights[e] = 1;
                edge_desc e2 = boost::add_edge(to, from, G).first; weights[e2] = 1;
            }
        }
    }

    int start_i = start.first*size_y + start.second;
    int end_i = end.first*size_y + end.second;


    vector<int> dist_from_start = dijkstra_dist(G, start_i);
    vector<int> dist_from_end = dijkstra_dist(G, end_i);
    int no_cheat_speed = dist_from_start[end_i];
    cout << "No cheat speed: " << no_cheat_speed << "\n";

    //Idea: for every pair i,j of track, check if their manhatten distance is <= 20
    //if yes, calculate dist_from_start[i] + dist_from_end[j] + manhatten_dist(i,j), if that is > 100 smaller than the no cheat speed, save it to cheat_times
    vector<pair<int, int>> cheat_pairs;
    vector<int> cheat_times;

    //for Part 1: set maxSteps to 2 
    //for Part 2: set maxSteps to 20
    int maxSteps = 20;
    int minSaved = 100;
    int sol = 0;

    std::map<int, int> cheat_savings_count;

    // Loop through all grid cells
    for (int i = 0; i < size_x; i++) {
        for (int j = 0; j < size_y; j++) {
            if (lines[j][i] == '#') continue;

            for (int k = std::max(0, i - maxSteps); k <= std::min(size_x - 1, i + maxSteps); k++) {
                for (int l = std::max(0, j - maxSteps); l <= std::min(size_y - 1, j + maxSteps); l++) {
                    if (k < 0 || k >= size_x || l < 0 || l >= size_y) continue;

                    if (lines[l][k] == '#') continue;

                    int manhattan_distance = abs(i - k) + abs(j - l);
                    if (manhattan_distance > maxSteps) continue;

                    int dist_start = dist_from_start[i * size_y + j];
                    int dist_end = dist_from_end[k * size_y + l];
                    if (dist_start < 0 || dist_end < 0) continue;

                    int cheat = dist_start + dist_end + manhattan_distance;
                    int savings = no_cheat_speed - cheat;

                    if (savings >= minSaved) {
                        sol++;
                        cheat_savings_count[savings]++;
                    }
                }
            }
        }
    }

    int final_result = 0;
    for (const auto& [savings, count] : cheat_savings_count) {
        cout << "There are " << count << " cheats that save " << savings << " picoseconds.\n";
        final_result += count;
    }
    cout << "Total: " << final_result << "\n";


    inFile.close();

    return 0;
}
