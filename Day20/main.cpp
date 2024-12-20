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

//Part 1 works, but it is painfully slow.. (mainly because i just copied dijkstra, but bfs would be enough)
//Part 2 tbd

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


    vector<pair<int,int>> cheat_paths_h;
    vector<pair<int,int>> cheat_paths_v;

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
            if(!nodes[x+1][y] && x+2 < size_x && nodes[x+2][y]){
                cheat_paths_h.push_back(make_pair(x+1, y));
            }
            if(!nodes[x][y+1] && y+2 < size_y && nodes[x][y+2]){
                cheat_paths_v.push_back(make_pair(x, y+1));
            }
        }
    }

    int start_i = start.first*size_y + start.second;
    int end_i = end.first*size_y + end.second;

    int no_cheat_speed = dijkstra_dist(G, start_i, end_i);
    cout << "No cheat speed: " << no_cheat_speed << "\n";

    //Cheat Horizontally:
    vector<int> time_saved;
    for(int i = 0; i<cheat_paths_h.size(); i++){
        int wall_x = cheat_paths_h[i].first;
        int wall_y = cheat_paths_h[i].second;
        //cheat in one direction
        int from = (wall_x-1)*size_y + wall_y;
        int to = wall_x*size_y + wall_y;
        edge_desc e = boost::add_edge(from, to, G).first; weights[e] = 1;
        from = wall_x*size_y + wall_y;
        to = (wall_x+1)*size_y + wall_y;
        edge_desc e2 = boost::add_edge(from, to, G).first; weights[e2] = 1;
        int cheat_speed = dijkstra_dist(G, start_i, end_i);
        if(no_cheat_speed - cheat_speed >= 100){
            time_saved.push_back(no_cheat_speed - cheat_speed);
        }
        //remove the edges e & e2 again
        boost::remove_edge(e, G);
        boost::remove_edge(e2, G);

        //cheat in opposite direction
        from = (wall_x+1)*size_y + wall_y;
        to = wall_x*size_y + wall_y;
        edge_desc e3 = boost::add_edge(from, to, G).first; weights[e3] = 1;
        from = wall_x*size_y + wall_y;
        to = (wall_x-1)*size_y + wall_y;
        edge_desc e4 = boost::add_edge(from, to, G).first; weights[e4] = 1;
        cheat_speed = dijkstra_dist(G, start_i, end_i);
        if(no_cheat_speed - cheat_speed >= 100){
            time_saved.push_back(no_cheat_speed - cheat_speed);
        }
        boost::remove_edge(e3, G);
        boost::remove_edge(e4, G);
    }

    //Cheat vertically:
    for(int i = 0; i<cheat_paths_v.size(); i++){
        int wall_x = cheat_paths_v[i].first;
        int wall_y = cheat_paths_v[i].second;
        //cheat in one direction
        int from = wall_x*size_y + wall_y - 1;
        int to = wall_x*size_y + wall_y ;
        edge_desc e = boost::add_edge(from, to, G).first; weights[e] = 1;
        from = wall_x*size_y + wall_y;
        to = wall_x*size_y + wall_y + 1;
        edge_desc e2 = boost::add_edge(from, to, G).first; weights[e2] = 1;
        int cheat_speed = dijkstra_dist(G, start_i, end_i);
        if(no_cheat_speed - cheat_speed >= 100){
            time_saved.push_back(no_cheat_speed - cheat_speed);
        }
        //remove the edges e & e2 again
        boost::remove_edge(e, G);
        boost::remove_edge(e2, G);

        //cheat in opposite direction
        from = wall_x*size_y + wall_y + 1;
        to = wall_x*size_y + wall_y;
        edge_desc e3 = boost::add_edge(from, to, G).first; weights[e3] = 1;
        from = wall_x*size_y + wall_y;
        to = wall_x*size_y + wall_y - 1;
        edge_desc e4 = boost::add_edge(from, to, G).first; weights[e4] = 1;
        cheat_speed = dijkstra_dist(G, start_i, end_i);
        if(no_cheat_speed - cheat_speed >= 100){
            time_saved.push_back(no_cheat_speed - cheat_speed);
        }
        //remove the edges e & e2 again
        boost::remove_edge(e3, G);
        boost::remove_edge(e4, G);
    }


    cout << time_saved.size() << "\n";

    inFile.close();

    return 0;
}
