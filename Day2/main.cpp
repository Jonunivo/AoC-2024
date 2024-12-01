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


    inFile.close();

    return 0;
}
