#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>

using namespace std;

// Open file helper function
ifstream openFile(const string& fileName) {
    ifstream inFile(fileName);
    if (!inFile) {
        cerr << "Error: Could not open " << fileName << "\n";
    }
    return inFile;
}

// Read file line by line
vector<string> readFileLineByLine(const string& fileName) {
    ifstream inFile = openFile(fileName);
    string line;
    vector<string> lines;
    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();
    return lines;
}

// Helper function to check if a subset is fully connected
void bronKerbosch(
    std::vector<std::string>& R,
    std::vector<std::string>& P,
    std::vector<std::string>& X,
    const std::unordered_map<std::string, std::unordered_set<std::string>>& neighbors,
    std::vector<std::vector<std::string>>& cliques) {

    if (P.empty() && X.empty()) {
        cliques.push_back(R); // Found a maximal clique
        return;
    }

    auto P_copy = P; // Copy P for iteration
    for (const auto& v : P_copy) {
        // Add v to R
        R.push_back(v);

        // Recurse with neighbors of v
        std::vector<std::string> newP, newX;
        for (const auto& u : P) {
            if (neighbors.at(v).count(u)) {
                newP.push_back(u);
            }
        }
        for (const auto& u : X) {
            if (neighbors.at(v).count(u)) {
                newX.push_back(u);
            }
        }

        bronKerbosch(R, newP, newX, neighbors, cliques);

        // Remove v from R and move it to X
        R.pop_back();
        P.erase(std::remove(P.begin(), P.end(), v), P.end());
        X.push_back(v);
    }
}

void findLargestClique(const std::unordered_map<std::string, std::vector<std::string>>& inputNeighbors) {
    // Convert neighbors to unordered_set for faster lookup
    std::unordered_map<std::string, std::unordered_set<std::string>> neighbors;
    for (const auto& [node, adjList] : inputNeighbors) {
        neighbors[node] = std::unordered_set<std::string>(adjList.begin(), adjList.end());
    }

    // All nodes in the graph
    std::vector<std::string> nodes;
    for (const auto& [node, _] : neighbors) {
        nodes.push_back(node);
    }

    // Find all maximal cliques
    std::vector<std::vector<std::string>> cliques;
    std::vector<std::string> R, P = nodes, X;
    bronKerbosch(R, P, X, neighbors, cliques);

    // Find the largest clique
    std::vector<std::string> largestClique;
    for (const auto& clique : cliques) {
        if (clique.size() > largestClique.size()) {
            largestClique = clique;
        }
    }

    // Output the largest clique
    std::cout << "Largest clique:\n";
    for (const auto& node : largestClique) {
        std::cout << node << " ";
    }
    std::cout << "\nEdges:\n";
    for (size_t i = 0; i < largestClique.size(); ++i) {
        for (size_t j = i + 1; j < largestClique.size(); ++j) {
            std::cout << largestClique[i] << "-" << largestClique[j] << '\n';
        }
    }

        // Generate the LAN party password
    std::sort(largestClique.begin(), largestClique.end());
    std::string password = "";
    for (size_t i = 0; i < largestClique.size(); ++i) {
        password += largestClique[i];
        if (i < largestClique.size() - 1) {
            password += ",";
        }
    }

    std::cout << "LAN Party Password: " << password << '\n';
}

void findTriangles(const std::unordered_map<std::string, std::vector<std::string>>& neighbors, std::set<std::vector<std::string>> &triangles) {

    for (const auto& [node, adjList] : neighbors) {
        for (size_t i = 0; i < adjList.size(); ++i) {
            for (size_t j = i + 1; j < adjList.size(); ++j) {
                const std::string& neighbor1 = adjList[i];
                const std::string& neighbor2 = adjList[j];

                // Check if neighbor1 and neighbor2 are connected
                if (std::find(neighbors.at(neighbor1).begin(), neighbors.at(neighbor1).end(), neighbor2) != neighbors.at(neighbor1).end()) {
                    // Sort the triangle to ensure uniqueness
                    std::vector<std::string> triangle = {node, neighbor1, neighbor2};
                    std::sort(triangle.begin(), triangle.end());
                    triangles.insert(triangle);
                }
            }
        }
    }
}

int main() {
    const string fileName = "input"; // Specify the file name
    const string path = fileName + ".txt";
    ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    //read input
    unordered_map<string, vector<string>> neighbors;
    for (const auto& line : lines) {
        std::istringstream iss(line);
        std::string first, second;
        size_t delimiter = line.find('-');
        if (delimiter != std::string::npos) {
            first = line.substr(0, delimiter);
            second = line.substr(delimiter + 1);
            if(neighbors.count(first) == 0){
                vector<string> curr = {second};
                neighbors[first] = curr;
            }
            else{
                neighbors[first].push_back(second);
            }
            if(neighbors.count(second) == 0){
                vector<string> curr = {first};
                neighbors[second] = curr;
            }
            else{
                neighbors[second].push_back(first);
            }
        }
    }

    std::set<std::vector<std::string>> triangles;
    set<vector<string>> t_triangles;

    findTriangles(neighbors, triangles);

    for (const auto& triangle : triangles) {
        std::cout << triangle[0] << "," << triangle[1] << "," << triangle[2] << '\n';
        if(triangle[0][0] == 't' || triangle[1][0] == 't' || triangle[2][0] == 't'){
            t_triangles.insert({triangle[0], triangle[1], triangle[2]});
        }
    }
    std::cout << "--------------------" << "\n";
    for (const auto& triangle : t_triangles) {
        std::cout << triangle[0] << "," << triangle[1] << "," << triangle[2] << '\n';
    }
    std::cout << "Result: " << t_triangles.size() << "\n";

    findLargestClique(neighbors);





 
    inFile.close();
    return 0;
}
