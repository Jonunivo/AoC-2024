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

void moveBoxes(int dir, vector<vector<int>>& grid, pair<int,int>& robopos){
    int i = robopos.first;
    int j = robopos.second;
    vector<pair<int,int>> toBeMoved;
    toBeMoved.push_back(make_pair(i,j));
    switch (dir){
        case 0: {
            while(grid[i-1][j] == 1){
                toBeMoved.push_back(make_pair(i-1, j));
                --i;
            }
            if(grid[i-1][j] == 0){
                bool first = true;
                for(auto it = toBeMoved.cbegin(); it != toBeMoved.cend(); ++it){
                    if(first){
                        robopos.first--;
                        grid[it->first][it->second] = 0;
                        grid[it->first-1][it->second] = 1;
                        first = false;
                    }
                    else{
                        grid[it->first-1][it->second] = 1;
                    }
                }
            }
            else if(grid[i-1][j] == -1){
                //wall at the end: don't move anything!
            }
            break;
        }
        case 1: {
            while(grid[i][j+1] == 1){
                toBeMoved.push_back(make_pair(i, j+1));
                cout << " see box ";
                ++j;
            }
            if(grid[i][j+1] == 0){
                cout << " see free ";

                bool first = true;
                for(auto it = toBeMoved.cbegin(); it != toBeMoved.cend(); ++it){
                    if(first){
                        robopos.second++;
                        grid[it->first][it->second] = 0;
                        grid[it->first][it->second+1] = 1;
                        first = false;
                    }
                    else{
                        grid[it->first][it->second+1] = 1;
                    }
                }
            }
            else if(grid[i][j+1] == -1){
                cout << " see wall ";
                //wall at the end: don't move anything!
            }
            break;
        }
        case 2: {
            while(grid[i+1][j] == 1){
                toBeMoved.push_back(make_pair(i+1, j));
                ++i;
            }
            if(grid[i+1][j] == 0){
                bool first = true;
                for(auto it = toBeMoved.cbegin(); it != toBeMoved.cend(); ++it){
                    if(first){
                        robopos.first++;
                        grid[it->first][it->second] = 0;
                        grid[it->first+1][it->second] = 1;
                        first = false;
                    }
                    else{
                        grid[it->first+1][it->second] = 1;
                    }
                }
            }
            else if(grid[i+1][j] == -1){
                //wall at the end: don't move anything!
            }
            break;
        }
        case 3: {
            while(grid[i][j-1] == 1){
                toBeMoved.push_back(make_pair(i, j-1));
                --j;
            }
            if(grid[i][j-1] == 0){
                bool first = true;
                for(auto it = toBeMoved.cbegin(); it != toBeMoved.cend(); ++it){
                    if(first){
                        robopos.second--;
                        grid[it->first][it->second] = 0;
                        grid[it->first][it->second-1] = 1;
                        grid[i][j-1] = 0;
                        first = false;
                    }
                    else{
                        grid[it->first][it->second-1] = 1;
                    }
                }
            }
            else if(grid[i][j-1] == -1){
                //wall at the end: don't move anything!
            }
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
                case 1: cout << "O"; break;
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

    //read input
    int size_i = lines[0].length();
    int size_j = size_i;
    pair<int,int> robopos;
    vector<vector<int>> grid(size_i, vector<int>(size_j));
    for(int i = 0; i<size_i; i++){
        for (int j = 0; j<size_j; j++) {
            switch (lines[i][j]) {
                case '.': grid[i][j] = 0; break;
                case 'O': grid[i][j] = 1; break;
                case '@': robopos = make_pair(i,j); break;
                case '#': grid[i][j] = -1; break;
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
