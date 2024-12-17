#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

//not my proudest code & blatantly inefficient, but it works...

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
    }

    inFile.close();
    return lines;
}

//returns:      0: no tile ahead (border reached)
//              1: object ahead
//              2: nothing ahead
int tileAhead(int direction, vector<vector<bool>> obstacles, int x, int y, int size_x, int size_y){
    switch(direction){
        case 0:
            if (x-1 < 0){
                return 0;
            }
            else if(obstacles[x-1][y]){
                return 1;
            }
            else{
                return 2;
            }
        case 1:
            if (y+1 > size_y-1){
                return 0;
            }
            else if(obstacles[x][y+1]){
                return 1;
            }
            else{
                return 2;
            }
        case 2:
            if (x+1 > size_x-1){
                return 0;
            }
            else if(obstacles[x+1][y]){
                return 1;
            }
            else{
                return 2;
            }
        case 3:
            if (y-1 < 0){
                return 0;
            }
            else if(obstacles[x][y-1]){
                return 1;
            }
            else{
                return 2;
            }
    }

}
//check if would run in loop
bool runSimulation(int direction, vector<vector<bool>> obstacles, int x, int y, int size_x, int size_y){
    int start_x = x;
    int start_y = y;
    int steps = 0;
    while(true){
        int toDo = tileAhead(direction, obstacles, x, y, size_x, size_y);
        if (toDo == 0){
            return false;
        }
        else if(toDo == 1){
            direction = (direction + 1)%4;
        }
        else{
            //step forward
            switch(direction){
                case 0:
                    --x;
                    break;
                case 1:
                    ++y;
                    break;
                case 2:
                    ++x;
                    break;
                case 3:
                    --y;
                    break;
            }
            steps++;
            if (steps > 10000){
                return true;
            }

        }
    }
}


int main() {
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    //matrix of obstacles
    const int size_x = lines.size();
    const int size_y = lines[0].size();
    vector<vector<bool>> obstacles(lines.size(), vector<bool>(lines[0].size(), false));
    //matrix of visited tiles
    vector<vector<bool>> visited(lines.size(), vector<bool>(lines[0].size(), false));

    //0 = ^, 1 = >, 2 = v, 3 = <;
    int start_direction = 0;
    int x_start = 0;
    int y_start = 0;
    // Iterate through each character in the line
    for (size_t i = 0; i < lines.size(); ++i) {
        const string &line = lines[i];
        for (size_t j = 0; j < line.size(); ++j) {
            char ch = line[j];

            // Check for specific characters
            if (ch == '.') {
                //do nothing
            } else if (ch == '#') {
                obstacles[i][j] = true;
            } else if (ch == '^') {
                visited[i][j] = true;
                start_direction = 0;
                x_start=i;
                y_start=j;
            } else if (ch == '>') {
                visited[i][j] = true;
                start_direction = 1;
                x_start=i;
                y_start=j;
            } else if (ch == 'v') {
                visited[i][j] = true;
                start_direction = 2;
                x_start=i;
                y_start=j;
            } else if (ch == '<') {
                visited[i][j] = true;
                start_direction = 3;
                x_start=i;
                y_start=j;
            }
        }
    }
    //walk until border reached
    int x = x_start;
    int y = y_start;
    int direction = start_direction;

    while(true){
        int toDo = tileAhead(direction, obstacles, x, y, size_x, size_y);
        if (toDo == 0){
            break;
        }
        else if(toDo == 1){
            direction = (direction + 1)%4;
        }
        else{
            //step forward
            switch(direction){
                case 0:
                    --x;
                    break;
                case 1:
                    ++y;
                    break;
                case 2:
                    ++x;
                    break;
                case 3:
                    --y;
                    break;
            }
            visited[x][y] = true;
        }
    }

    //count how much visited:
    int count = 0;
    int count2 = 0;

    for(int i = 0; i<size_x; i++){
        for(int j = 0; j<size_y; j++){
            if(visited[i][j]){
                if(i == x_start && j == y_start){
                    continue;
                }
                //add obstacle
                obstacles[i][j] = true;
                if(runSimulation(start_direction, obstacles, x_start, y_start, size_x, size_y)){
                    count2++;
                }
                obstacles[i][j] = false;

                count++;
                cout << count << "\n";
            }
        }
    }

    cout << count << " " << count2;

    

    return 0;
}
