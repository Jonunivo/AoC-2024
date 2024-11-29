#include <fstream>
#include <iostream>
#include <string>

// Function to open a file and return the input file stream
std::ifstream openFile(const std::string& fileName) {
    std::ifstream inFile(fileName);
    if (!inFile) {
        std::cerr << "Error: Could not open " << fileName << "\n";
    }
    return inFile;
}

int main() {
    const std::string fileName = "input.txt"; // Specify the file name
    std::ifstream inFile = openFile(fileName);
    if (!inFile.is_open()) {
        return 1; 
    }
    //read line by line
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl; 
    }

    //MAIN

    inFile.close();
    return 0;
}
