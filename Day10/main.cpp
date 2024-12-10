#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

//This file outputs   6289587115689 for part 2 (too high)
//the actual solution 6289564433984 

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


int findPath(vector<vector<bool>>& visited, vector<vector<int>>& height_map, int i, int max_i, int j, int max_j, int height){
    int returnVal = 0;
    if(height == 9){
        visited[i][j] = true;
        return 1;
    }
    if(i > 0 && height_map[i-1][j] == height+1){
        returnVal += 0 + findPath(visited, height_map, i-1, max_i, j, max_j, height+1);
    }
    if(i+1 < max_i && height_map[i+1][j] == height+1){
        returnVal += 0 + findPath(visited, height_map, i+1, max_i, j, max_j, height+1);
    }
    if(j > 0 && height_map[i][j-1] == height+1){
        returnVal += 0 + findPath(visited, height_map, i, max_i, j-1, max_j, height+1);
    }
    if(j+1 < max_j && height_map[i][j+1] == height+1){
        returnVal += 0 + findPath(visited, height_map, i, max_i, j+1, max_j, height+1);
    }
    return returnVal;
}

int main() {
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    //Step 1: Input -> Hiking Map
    int max_j = lines[0].size();
    int max_i = lines.size();
    vector<vector<int>> height_map(lines.size(), vector<int>(lines[0].size(), -1));
    for(int i = 0; i<max_i; i++){
        string line = lines[i];
        for(int j = 0; j<max_j; j++){
            int curr = line[j] - '0';
            height_map[i][j] = curr;
        }
    }

    vector<vector<int>> paths;
    int start_point_i = 0;
    int height_level = 0;
    int score2 = 0;
    int score1= 0;
    for(int i = 0; i<max_i; i++){
        for(int j = 0; j<max_j; j++){  
            if(height_map[i][j] == 0){
                //find adjacent height levels
                vector<vector<bool>> visited(max_i, vector<bool>(max_j, false));
                int curr_score2 = findPath(visited, height_map, i, max_i, j, max_j, 0);
                int curr_score1 = 0;
                for(int i = 0; i<max_i; i++){
                    for(int j = 0; j<max_j; j++){  
                        if(visited[i][j]){
                            curr_score1++;
                        }
                    }
                }

                cout << i << " "  << j << " " << curr_score2 << " " << curr_score1 << "\n";
                score2 += curr_score2;
                score1 += curr_score1;
            }
        }
    }

    cout << "Result1: " << score1 << "\n";
    cout << "Result2: " << score2 << "\n";



    inFile.close();

    return 0;
}
