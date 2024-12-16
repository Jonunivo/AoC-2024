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
#include <set>

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

void moveRobot(int dir, vector<vector<int>>& grid, pair<int,int>& robopos){
    grid[robopos.first][robopos.second] = 0;
    switch (dir){
        case 0: robopos.first--; break;
        case 1: robopos.second++; break;
        case 2: robopos.first++; break;
        case 3: robopos.second--; break;
    }
    return;
}

void moveUp(vector<vector<int>>& grid,  vector<pair<int,int>>& toBeMoved, pair<int,int>& robopos){
    int i = robopos.first;
    int j = robopos.second;
    bool found_wall = false;
    bool move = false;
    //Collect Boxes above
    set<int> nextLayerJ;
    nextLayerJ.insert(j);
    //tile above @ = [
    if(grid[i-1][j] == 1){
        nextLayerJ.insert(j+1);
    }
    //tile above @ = ]
    else if(grid[i-1][j] == 2){
        nextLayerJ.insert(j-1);
    }
    i--;
    while(true){
        int num_empty = 0;
        set<int> nextnextLayerJ;
        for(auto it = nextLayerJ.cbegin(); it != nextLayerJ.cend(); it++){
            toBeMoved.push_back(make_pair(i, *it));
            switch (grid[i-1][*it]){
                case -1: found_wall = true; break;
                case 1: nextnextLayerJ.insert(*it); nextnextLayerJ.insert(*it + 1); break;
                case 2: nextnextLayerJ.insert(*it); nextnextLayerJ.insert(*it - 1); break;
                case 3: num_empty++; break;
            }
        }
        if(num_empty == nextLayerJ.size()){
            //all above are empty. Do the moves!
            move = true;
            break;
        }
        if(found_wall){
            //found a wall above. Don't move!
            move = false;
            break;
        }
        nextLayerJ = nextnextLayerJ;
        i--;
    }
    //do the moves
    // Move the robot
    if(move){
        int robotI = toBeMoved[0].first;
        int robotJ = toBeMoved[0].second;
        robopos.first--;                // Update robot's position
        grid[robotI][robotJ] = 0;       // Clear the current robot position
        toBeMoved.erase(toBeMoved.begin()); // Remove the robot's position from toBeMoved

        // Sort the remaining positions to ensure proper box movement
        sort(toBeMoved.begin(), toBeMoved.end());

        // Move the boxes
        for (const auto& pos : toBeMoved) {
            int curI = pos.first;
            int curJ = pos.second;

            int boxShape = grid[curI][curJ]; // Determine the shape of the box (1 or 2)
            grid[curI][curJ] = 0;           // Clear the current box position
            grid[curI - 1][curJ] = boxShape; // Move the box with the same shape
        }
    }

}

void moveDown(vector<vector<int>>& grid, vector<pair<int,int>>& toBeMoved, pair<int,int>& robopos) {
    int i = robopos.first;
    int j = robopos.second;
    bool found_wall = false;
    bool move = false;
    
    // Collect Boxes below
    set<int> nextLayerJ;
    nextLayerJ.insert(j);
    // Tile below @ = [
    if (grid[i+1][j] == 1) {
        nextLayerJ.insert(j+1);
    }
    // Tile below @ = ]
    else if (grid[i+1][j] == 2) {
        nextLayerJ.insert(j-1);
    }
    i++;
    while (true) {
        int num_empty = 0;
        set<int> nextnextLayerJ;
        for (auto it = nextLayerJ.cbegin(); it != nextLayerJ.cend(); it++) {
            toBeMoved.push_back(make_pair(i, *it));
            switch (grid[i+1][*it]) {
                case -1: 
                    found_wall = true; 
                    break;
                case 1: 
                    nextnextLayerJ.insert(*it); 
                    nextnextLayerJ.insert(*it + 1); 
                    break;
                case 2: 
                    nextnextLayerJ.insert(*it); 
                    nextnextLayerJ.insert(*it - 1); 
                    break;
                case 3: 
                    num_empty++; 
                    break;
            }
        }
        if (num_empty == nextLayerJ.size()) {
            // All below are empty. Do the moves!
            move = true;
            break;
        }
        if (found_wall) {
            // Found a wall below. Don't move!
            move = false;
            break;
        }
        nextLayerJ = nextnextLayerJ;
        i++;
    }

    // Perform the moves
    if (move) {
        // Move the robot
        int robotI = toBeMoved[0].first;
        int robotJ = toBeMoved[0].second;
        robopos.first++;                // Update robot's position
        grid[robotI][robotJ] = 0;       // Clear the current robot position
        toBeMoved.erase(toBeMoved.begin()); // Remove the robot's position from toBeMoved

        // Sort the remaining positions to ensure proper box movement
        sort(toBeMoved.begin(), toBeMoved.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.first > b.first; // Sort in descending order by row for downward movement
        });

        // Move the boxes
        for (const auto& pos : toBeMoved) {
            int curI = pos.first;
            int curJ = pos.second;

            int boxShape = grid[curI][curJ]; // Determine the shape of the box (1 or 2)
            grid[curI][curJ] = 0;           // Clear the current box position
            grid[curI + 1][curJ] = boxShape; // Move the box with the same shape
        }
    }
}

void moveRight(vector<vector<int>>& grid,  vector<pair<int,int>>& toBeMoved, pair<int,int>& robopos){
    int i = robopos.first;
    int j = robopos.second;
    while(grid[i][j+1] == 1 || grid[i][j+1] == 2){
        toBeMoved.push_back(make_pair(i, j+1));
        ++j;
    }
    if(grid[i][j+1] == 0){
        bool first = true;
        int count = 0;
        for(auto it = toBeMoved.cbegin(); it != toBeMoved.cend(); ++it){
            if(first){
                robopos.second++;
                grid[it->first][it->second] = 0;
                grid[it->first][it->second+1] = 1;
                first = false;
            }
            else{
                if(count%2 == 0){
                    grid[it->first][it->second+1] = 1;
                }
                else{
                    grid[it->first][it->second+1] = 2;
                }
                count++;
            }
        }
    }
    else if(grid[i][j+1] == -1){
        //wall at the end: don't move anything!
    }
    return;
}

void moveLeft(vector<vector<int>>& grid,  vector<pair<int,int>>& toBeMoved, pair<int,int>& robopos){
    int i = robopos.first;
    int j = robopos.second;
    while(grid[i][j-1] == 1 || grid[i][j-1] == 2){
        toBeMoved.push_back(make_pair(i, j-1));
        --j;
    }
    if(grid[i][j-1] == 0){
        bool first = true;
        int count = 0;
        for(auto it = toBeMoved.cbegin(); it != toBeMoved.cend(); ++it){
            if(first){
                robopos.second--;
                grid[it->first][it->second] = 0;
                grid[it->first][it->second-1] = 2;
                grid[i][j-1] = 0;
                first = false;
            }
            else{
                if(count%2 == 0){
                    grid[it->first][it->second-1] = 2;
                }
                else{
                    grid[it->first][it->second-1] = 1;
                }
                count++;
            }
        }
    }
    else if(grid[i][j-1] == -1){
        //wall at the end: don't move anything!
    }

}
void moveBoxes(int dir, vector<vector<int>>& grid, pair<int,int>& robopos){
    int i = robopos.first;
    int j = robopos.second;
    vector<pair<int,int>> toBeMoved;
    toBeMoved.push_back(make_pair(i,j));
    switch (dir){
        case 0: {
            moveUp(grid, toBeMoved, robopos);
            break;
        }
        case 1: {
            moveRight(grid, toBeMoved, robopos);
            break;
        }
        case 2: {
            moveDown(grid, toBeMoved, robopos);
            break;
        }
        case 3: {
            moveLeft(grid, toBeMoved, robopos);
            break;
        }
    }
    return;
}


void trymove(int dir, vector<vector<int>>& grid, pair<int,int>& robopos){
    int tileAhead;
    int i = robopos.first;
    int j = robopos.second;
    switch (dir){
        case 0: tileAhead = grid[i-1][j]; break;
        case 1: tileAhead = grid[i][j+1]; break;
        case 2: tileAhead = grid[i+1][j]; break;
        case 3: tileAhead = grid[i][j-1]; break;
    }
    cout << "dir: "<< dir << " ta: " << tileAhead << "\n";
    switch(tileAhead){
        case 0: moveRobot(dir, grid, robopos); break;
        case 1: moveBoxes(dir, grid, robopos); break;
        case 2: moveBoxes(dir, grid, robopos); break;
        case -1: break; //ignore 
    }
    return;
}

void printGrid(int dir, vector<vector<int>>& grid, pair<int,int>& robopos, int size_i, int size_j){
    for(int i = 0; i<size_i; i++){
        for(int j = 0; j<size_j; j++){
            if(i == robopos.first && j == robopos.second){
                cout << "@";
                continue;
            }
            switch(grid[i][j]){
                case 0: cout << ".";break;
                case 1: cout << "["; break;
                case 2: cout << "]"; break;
                case -1: cout << "#"; break;

            }

        }
        cout << "\n";
    }
}



int main() {
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    //read input & duplicate grid
    int size_i = lines[0].length();
    int size_j = size_i*2;
    pair<int,int> robopos;
    vector<vector<int>> grid(size_i, vector<int>(size_j));
    for(int i = 0; i<size_i; i++){
        for (int j = 0; j<size_j; j+=2) {
            switch (lines[i][j/2]) {
                case '.': grid[i][j] = 0; grid[i][j+1] = 0; break;
                case 'O': grid[i][j] = 1; grid[i][j+1] = 2; break;
                case '@': robopos = make_pair(i,j); grid[i][j] = 0; grid[i][j+1] = 0; break;
                case '#': grid[i][j] = -1; grid[i][j+1] = -1; break;
            }
        }
    }

    vector<int> directions;
    for(int i = size_i+1; i<lines.size(); ++i){
        for(int j = 0; j<lines[size_i+1].size(); j++){
            switch(lines[i][j]){
                case '^': directions.push_back(0); break;
                case '>': directions.push_back(1); break;
                case 'v': directions.push_back(2); break;
                case '<': directions.push_back(3); break;
            }
        }
    }

    //do steps
    printGrid(0, grid, robopos, size_i, size_j);

    for(auto it = directions.cbegin(); it != directions.cend(); it++){
        trymove(*it, grid, robopos);
        switch (*it){
            case 0: cout << "move" << "^" << "\n"; break;
            case 1: cout << "move" << ">" << "\n"; break;
            case 2: cout << "move" << "v" << "\n"; break;
            case 3: cout << "move" << "<" << "\n"; break;
        }
        //printGrid(0, grid, robopos, size_i, size_j);
    }
    printGrid(0, grid, robopos, size_i, size_j);


    //print output
    int result = 0;
    for(int i = 0; i<size_i; i++){
        for(int j = 0; j<size_j; j++){
            if(grid[i][j] == 1){
                result += 100*i + j;
            }
        }
    }

    cout << result << "\n"; 




    
    inFile.close();

    return 0;
}
