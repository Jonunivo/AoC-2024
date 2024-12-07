#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <cmath>


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

long long concatenate(long long left, long long right) {
    long digits = (right == 0) ? 1 : static_cast<long>(log10(right) + 1);
    return left * static_cast<long long>(pow(10, digits)) + right;
}




int main() {
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    long long final_result = 0;

    for(string line : lines){
        //Input
        std::istringstream iss(line);
        long goal;
        char colon;
        iss >> goal >> colon;
        std::vector<int> numbers;
        int number;
        while (iss >> number) {
            numbers.push_back(number);
        }

        size_t n = numbers.size(); // Size of the vector
        size_t numStates = 1;
        for (size_t i = 0; i < n; ++i) {
            numStates *= 3; // 3^n states
        }

        std::vector<int> states(n, 0); // State can now hold values 0, 1, 2


        for(int i = 0; i<numStates; i++){
            //check this state
            
            long long result = numbers[0];
            for(int i = 1; i<numbers.size(); i++){
                if(states[i] == 0){
                    result += numbers[i];
                }
                else if(states[i] == 1){
                    result *= numbers[i];
                }
                else{
                    long long result_old = result;
                    result = concatenate(result, numbers[i]);
                }
            }
            if(result == goal){
                final_result += goal;
                break;
            }
            
            //move to next state
            for (size_t j = n; j-- > 0;) {
                if (states[j] < 2) {
                    states[j] += 1;
                    break;
                } else {
                    states[j] = 0;
                }
            }
        }
    

    }

    cout << final_result << "\n";

 
    return 0;
}
