#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
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
        std::cout << line << std::endl;
    }

    inFile.close();
    return lines;
}

int main() {
    const std::string fileName = "input"; // Specify the file name
    const std::string path = "C:/Users/uniho/OneDrive/Documents/AoC-2024/Day1/" + fileName + ".txt";
    std::ifstream inFile = openFile(path);

    // MAIN: Add your logic here
    vector<int> firstCol;
    vector<int> secondCol;
    int first, second;
    while(inFile >> first >> second){
        firstCol.push_back(first);
        secondCol.push_back(second);
    }
    sort(firstCol.begin(), firstCol.end());
    sort(secondCol.begin(), secondCol.end());

    int64_t difference = 0;
    for(int i = 0; i<firstCol.size(); i++){
        int curr = firstCol[i];
        int count = 0;
        for(int j = 0; j<secondCol.size(); j++){
            if(secondCol[j] == curr){
                count++;
            }
        }
        difference += curr * count;
    }
    cout << difference;

    inFile.close();

    return 0;
}
