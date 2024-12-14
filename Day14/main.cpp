#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <tuple>
#include <fstream>

using namespace std;

int grid_size_x = 103;
int grid_size_y = 101;

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

void print_grid_to_file(const vector<vector<int>>& num_robots, int grid_size_x, int grid_size_y, const string& filename, int step) {
    // Open the file in append mode
    std::ofstream file(filename, std::ios::app); 

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    // Print the step number as a header
    file << "Step " << step << ":\n";

    // Write the grid
    for (int i = 0; i < grid_size_x; i++) {
        for (int j = 0; j < grid_size_y; j++) {
            if (num_robots[i][j] == 0) {
                file << ".";
            } else {
                file << num_robots[i][j];
            }
        }
        file << "\n"; // End of row
    }
    file << "----------------------\n"; // Separator

    file.close(); // Close the file
}

bool check_full_grid(const vector<vector<int>>& num_robots, int grid_size_x, int grid_size_y) {
    // Iterate over all possible 10x10 sub-grids
    for (int i = 0; i <= grid_size_x - 5; ++i) {        // Top-left corner row
        for (int j = 0; j <= grid_size_y - 5; ++j) {    // Top-left corner column
            bool all_cells_filled = true;

            // Check the 5x5 grid starting at (i, j)
            for (int x = 0; x < 5; ++x) {
                for (int y = 0; y < 5; ++y) {
                    if (num_robots[i + x][j + y] < 1) {  // Cell is empty
                        all_cells_filled = false;
                        break;
                    }
                }
                if (!all_cells_filled) break; // Exit early if any cell fails
            }

            // If we found a valid 10x10 grid, return true
            if (all_cells_filled) {
                return true;
            }
        }
    }

    // No valid 10x10 grid found
    return false;
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
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    //read input
    vector<vector<int>> num_robots(grid_size_x, vector<int>(grid_size_y, 0));
    vector<tuple<int,int,int,int>> robots;
    for(auto line : lines){
        std::stringstream ss(line);
        char ignore;
        int x1, y1, x2, y2;
        ss >> ignore >> ignore >> y1 >> ignore >> x1; // Parse "p=x,y"
        ss >> ignore >> ignore >> y2 >> ignore >> x2; // Parse "v=x,y"
        num_robots[x1][y1]++;
        robots.push_back({x1, y1, x2, y2});
    }


    int count = 0;
    for(int k = 0 ; k<10000; k++){
        simulate_step(num_robots, robots);

        //print
        if(check_full_grid(num_robots, grid_size_x, grid_size_y)){
            count++;
            print_grid_to_file(num_robots, grid_size_x, grid_size_y, "output.txt", k+1);
        }
    }

    //print result
    int top_right = 0, top_left= 0, bot_left = 0, bot_right = 0;
    for(int i = 0; i<grid_size_x; i++){
        for(int j = 0; j<grid_size_y; j++){
            if(i < grid_size_x / 2 && j < grid_size_y / 2){
                top_left += num_robots[i][j];
            }
            else if(i > grid_size_x / 2 && j < grid_size_y / 2){
                top_right += num_robots[i][j];
            }
            else if(i < grid_size_x / 2 && j > grid_size_y / 2){
                bot_left += num_robots[i][j];
            }
            else if(i > grid_size_x / 2 && j > grid_size_y / 2){
                bot_right += num_robots[i][j];
            }
        }
    }
    cout << top_left << " " << top_right << " " << bot_left << " " << bot_right << "\n";
    cout << top_left*top_right*bot_left*bot_right << "\n";
    cout << count << "\n";


 
    inFile.close();

    return 0;
}
