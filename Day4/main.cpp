#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

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

int main() {
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    vector<vector<char>> grid;
    for(string line : lines){
        vector<char> row;
        for(char c : line){
            row.push_back(c);
        }
        grid.push_back(row);
    }

    int numRows = grid.size();
    int numCols = (numRows > 0) ? grid[0].size() : 0;
    
    int count = 0;
    for(int i = 0; i<numRows; i++){
        for(int j = 0; j<numCols; j++){
            /* Part 1 
            if(grid[i][j] == 'X'){
                //Check ->
                if(j+3 < numCols && grid[i][j+1] == 'M' && grid[i][j+2] == 'A' && grid[i][j+3] == 'S'){
                    count++;
                }
                //Check <-
                if(j-3 >= 0 && grid[i][j-1] == 'M' && grid[i][j-2] == 'A' && grid[i][j-3] == 'S'){
                    count++;
                }
                //Check diag_UL
                if(j-3 >= 0 && i-3 >= 0 && grid[i-1][j-1] == 'M' && grid[i-2][j-2] == 'A' && grid[i-3][j-3] == 'S'){
                    count++;
                }
                //Check diag_UR
                if(j+3 < numCols && i-3 >= 0 && grid[i-1][j+1] == 'M' && grid[i-2][j+2] == 'A' && grid[i-3][j+3] == 'S'){
                    count++;
                }
                //Check diag_DL
                if(j-3 >= 0 && i+3 < numRows && grid[i+1][j-1] == 'M' && grid[i+2][j-2] == 'A' && grid[i+3][j-3] == 'S'){
                    count++;
                }
                //Check diag_DR
                if(j+3 < numCols && i+3 < numRows && grid[i+1][j+1] == 'M' && grid[i+2][j+2] == 'A' && grid[i+3][j+3] == 'S'){
                    count++;
                }
                //Check up
                if(i-3 >= 0 && grid[i-1][j] == 'M' && grid[i-2][j] == 'A' && grid[i-3][j] == 'S'){
                    count++;
                }
                //Check down
                if(i+3 < numRows && grid[i+1][j] == 'M' && grid[i+2][j] == 'A' && grid[i+3][j] == 'S'){
                    count++;
                }
            }
            */
           //Part 2
            if(grid[i][j] == 'A' && i+1 < numRows && j+1 < numCols && i-1 > -1 && j-1 > -1){
                //MM SS
                if(grid[i-1][j-1] == 'M' && grid[i-1][j+1] == 'M' &&
                    grid[i+1][j-1] == 'S' && grid[i+1][j+1] == 'S'){
                    count++;
                }
                //MS MS
                if(grid[i-1][j-1] == 'M' && grid[i-1][j+1] == 'S' &&
                    grid[i+1][j-1] == 'M' && grid[i+1][j+1] == 'S'){
                    count++;
                }
                //SM SM
                if(grid[i-1][j-1] == 'S' && grid[i-1][j+1] == 'M' &&
                    grid[i+1][j-1] == 'S' && grid[i+1][j+1] == 'M'){
                    count++;
                }
                //SS MM
                if(grid[i-1][j-1] == 'S' && grid[i-1][j+1] == 'S' &&
                    grid[i+1][j-1] == 'M' && grid[i+1][j+1] == 'M'){
                    count++;
                }

            }

        }
    }

    cout << "count: " << count;


    inFile.close();

    return 0;
}
