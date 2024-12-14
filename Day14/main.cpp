#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <tuple>

using namespace std;

int grid_size_x = 7;
int grid_size_y = 11;

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

void simulate_step(vector<vector<int>>& num_robots,  vector<tuple<int,int,int,int>>& robots){
    //go over each robot & move one step
    for(auto it = robots.begin(); it != robots.end(); it++){
        int x = get<0>(*it);
        int y = get<1>(*it);
        int v_x = get<2>(*it);
        int v_y = get<3>(*it);

        num_robots[x][y]--;
        x += v_x % grid_size_x;
        y += v_y % grid_size_y;
        //assure nonegativity:
        x = (x + grid_size_x) % grid_size_x;
        y = (y + grid_size_y) % grid_size_y;

        get<0>(*it) = x;
        get<1>(*it) = y;
        num_robots[x][y]++;
    }
}

int main() {
    const std::string fileName = "sample"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    //read input
    vector<vector<int>> num_robots(7, vector<int>(11, 0));
    vector<tuple<int,int,int,int>> robots;
    for(auto line : lines){
        std::stringstream ss(line);
        char ignore;
        int x1, y1, x2, y2;
        ss >> ignore >> ignore >> x1 >> ignore >> y1; // Parse "p=x,y"
        ss >> ignore >> ignore >> x2 >> ignore >> y2; // Parse "v=x,y"
        num_robots[x1][y1]++;
        robots.push_back({x1, y1, x2, y2});
    }

    for(int k = 0 ; k<10; k++){
        simulate_step(num_robots, robots);
        //print
        for(int i = 0; i<grid_size_x; i++){
            for(int j = 0; j<grid_size_y; j++){
                if(num_robots[i][j] == 0){
                    cout << ".";
                }
                else{
                    cout << num_robots[i][j];
                }
            }
            cout << "\n";
        }
        cout << "----------------------" << "\n";
    }


 
    inFile.close();

    return 0;
}
