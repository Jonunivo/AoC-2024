#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>

//Note: this code is pretty ugly, but it works..
//FOR PART 2: THE LINE COUNTING IS NOT QUITE CORRECT (works on every single sample, but too low (837075 instead of 839780) for input...)

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

//returns num_fields, perimeter_size
pair<int,int> bfs(vector<pair<int,int>> &perimeter_map_vertical, vector<pair<int,int>> &perimeter_map_horizontal, vector<vector<bool>>& visited, vector<vector<char>>& grid, int i, int j, int maxi, int maxj){
    //do BFS (region growth)
    int perimeter_size = 0;
    int num_fields = 1;
    char c = grid[i][j];
    //(x,y)
    queue<pair<int,int>> queue;
    visited[i][j] = true;
    queue.push(make_pair(i,j));
    while(!queue.empty()){
        pair<int,int> curr = queue.front();
        queue.pop();
        int curri = curr.first;
        int currj = curr.second;
        //check up
        if(curri - 1 >= 0 && !visited[curri - 1][currj]){
            if(grid[curri - 1][currj] == c){
                visited[curri - 1][currj] = true;
                queue.push(make_pair(curri - 1,currj));
                num_fields++;
            }
            else{
                perimeter_size++;
                perimeter_map_horizontal.push_back(make_pair(curri - 1, currj));
            }
        }
        else if(curri - 1 < 0 || grid[curri - 1][currj] != c){
            perimeter_size++;
            perimeter_map_horizontal.push_back(make_pair(curri - 1, currj));
        }
        //check down
        if(curri + 1 < maxi && !visited[curri + 1][currj]){
            if(grid[curri + 1][currj] == c){
                visited[curri + 1][currj] = true;
                queue.push(make_pair(curri + 1,currj));
                num_fields++;
            }
            else{
                perimeter_size++;
                perimeter_map_horizontal.push_back(make_pair(curri + 1, currj));
            }
        }
        else if(curri + 1 >= maxi || grid[curri + 1][currj] != c){
            perimeter_size++;
            perimeter_map_horizontal.push_back(make_pair(curri + 1, currj));
        }
        //check left
        if(currj - 1 >= 0 && !visited[curri][currj - 1]){
            if(grid[curri][currj - 1] == c){
                visited[curri][currj - 1] = true;
                queue.push(make_pair(curri,currj - 1));
                num_fields++;
            }
            else{
                perimeter_size++;
                perimeter_map_vertical.push_back(make_pair(curri, currj - 1));
            }
        }
        else if(currj - 1 < 0 || grid[curri][currj - 1] != c){
            perimeter_size++;
            perimeter_map_vertical.push_back(make_pair(curri, currj - 1));
        }
        //check right
        if(currj + 1 < maxj && !visited[curri][currj + 1]){
            if(grid[curri][currj+1] == c){
                visited[curri][currj+1] = true;
                queue.push(make_pair(curri,currj+1));
                num_fields++;
            }
            else{
                perimeter_size++;
                perimeter_map_vertical.push_back(make_pair(curri, currj + 1));

            }
        }
        else if(currj + 1 >= maxj || grid[curri][currj + 1] != c) {
            perimeter_size++;
            perimeter_map_vertical.push_back(make_pair(curri, currj + 1));

        }
    }
    return make_pair(num_fields, perimeter_size);
}

int countSidesHorizontal(vector<pair<int,int>> &perimeter_map){
    int counter = 0;

    // Sort the perimeter_map to group duplicates together
    std::sort(perimeter_map.begin(), perimeter_map.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        if (a.first == b.first) {
            return a.second < b.second;
        }
        return a.first < b.first;
    });

    // Iterate through the vector and handle duplicates
    for (size_t i = 1; i < perimeter_map.size(); ++i) {
        if (perimeter_map[i] == perimeter_map[i - 1]) {
            perimeter_map[i].first += 100000;
        }
    }

    //horizontal
    std::sort(perimeter_map.begin(), perimeter_map.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        if (a.first == b.first) {
            return a.second < b.second;
        }
        return a.first < b.first;
    });

    for (auto it = perimeter_map.begin(); it != perimeter_map.end(); ++it) {
        auto next_it = std::next(it); 
        if (next_it != perimeter_map.end()) {
            if (next_it->first != it->first) {
                //vertical step
                counter++;
            }
            else if (next_it->second != it->second + 1) {
                //gap > 1
                counter++;
            }
        }
        else {
            // Last element in the row
            counter++;
        }
    }
    return counter;

}
int countSidesVertical(vector<pair<int,int>> &perimeter_map){
    int counter = 0;
    //vertical
    std::sort(perimeter_map.begin(), perimeter_map.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        if (a.second == b.second) {
            return a.first < b.first;
        }
        return a.second < b.second;
    });

    // Iterate through the vector and handle duplicates
    for (size_t i = 1; i < perimeter_map.size(); ++i) {
        if (perimeter_map[i] == perimeter_map[i - 1]) {
            perimeter_map[i].second += 100000;
        }
    }

    //resort
    std::sort(perimeter_map.begin(), perimeter_map.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        if (a.second == b.second) {
            return a.first < b.first;
        }
        return a.second < b.second;
    });

    for (auto it = perimeter_map.begin(); it != perimeter_map.end(); ++it) {
        auto next_it = std::next(it); 
        if (next_it != perimeter_map.end()) {
            if (next_it->second != it->second) {
                //horizontal step
                counter++;
            }
            else if (next_it->first != it->first + 1) {
                //gap > 1
                counter++;
            }
        }
        else {
            // Last element in the column
            counter++;
        }
    }
    return counter;
}

int main() {
    const std::string fileName = "sample"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    //read input
    int maxi = lines.size();
    int maxj = lines[0].size();
    vector<vector<char>> grid(maxi, vector<char>(maxj));
    for(int i = 0; i<maxi; i++){
        for(int j = 0; j<maxj; j++){
            grid[i][j] = lines[i][j];
        }
    }

    vector<vector<bool>> visited(maxi, vector<bool>(maxj, false));
    //BFS 
    vector<pair<int,int>> perimeter_map_vertical;
    vector<pair<int,int>> perimeter_map_horizontal;
    int final_result = 0;
    for(int i = 0; i<maxi; i++){
        for(int j = 0; j<maxj; j++){
            if(visited[i][j]){
                continue;
            }
            perimeter_map_vertical.clear();
            perimeter_map_horizontal.clear();

            pair<int,int> result = bfs(perimeter_map_vertical, perimeter_map_horizontal, visited, grid, i, j, maxi, maxj);
            //count sides of perimeter_map
            int side_count = countSidesVertical(perimeter_map_vertical) + countSidesHorizontal(perimeter_map_horizontal);
            cout << i << " " << j << ": " << grid[i][j] << " | " << result.first << " | " << countSidesVertical(perimeter_map_vertical) << " " << countSidesHorizontal(perimeter_map_horizontal) << "\n";

            final_result += result.first * side_count;
        }
    }


    cout << final_result << "\n";
 
    inFile.close();

    return 0;
}
