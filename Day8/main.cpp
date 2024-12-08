#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

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

    int numCols = lines[0].size();
    int numRows = lines.size();

    //Char -> [x,y], [x,y], ...
    unordered_map<char, vector<pair<int,int>>> antennas;
    
    for(int i = 0; i<numRows; i++){
        string line = lines[i];

        for(int j = 0; j<numCols; j++){
            if(line[j] == '.'){
                //ignore
            }
            else{
                //read character
                char c = line[j];
                if(antennas.count(c) == 0){
                    vector<pair<int,int>> currLoc;
                    currLoc.push_back({i, j});
                    antennas[c] = currLoc;
                }
                else{
                    antennas[c].push_back({i,j});
                }
            }
        }
    }

    vector<vector<bool>> antinodes(numRows, vector<bool>(numCols, false));

    for(auto it = antennas.cbegin(); it != antennas.cend(); it++){
        vector<pair<int,int>> positions = it->second;
        for(int i = 0; i < positions.size(); i++){
            for(int j = i+1; j<positions.size(); j++){
                int firstPosX = positions[i].first;
                int firstPosY = positions[i].second;
                int secondPosX = positions[j].first;
                int secondPosY = positions[j].second;
                int deltaX = secondPosX - firstPosX;
                int deltaY = secondPosY - firstPosY;
                //set antinodes
                while(firstPosX >= 0 && firstPosX < numRows && firstPosY >= 0 && firstPosY < numCols){
                    antinodes[firstPosX][firstPosY] = true;
                    firstPosX -= deltaX;
                    firstPosY -= deltaY;
                }
                while(secondPosX >= 0 && secondPosX < numRows && secondPosY >= 0 && secondPosY < numCols){
                    antinodes[secondPosX][secondPosY] = true;
                    secondPosX += deltaX;
                    secondPosY += deltaY;
                }
            }
        }
    }

    int count = 0;
    for(int i = 0; i<numRows; i++){
        for(int j = 0; j<numCols; j++){
            if(antinodes[i][j]){
                cout << "#";
                count++;
            }
            else{
                cout << ".";
            }
        }
        cout << "\n";
    }
    cout << count << "\n";


    inFile.close();

    return 0;
}
