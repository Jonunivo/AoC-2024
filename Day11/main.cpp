#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

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

long long numDigits(long long number){
    return floor(std::log10(number)) + 1;
}

pair<long long, long long> splitEven(long long number){
    long long digitCount = static_cast<long long>(std::log10(number)) + 1;
    long halfDigits = digitCount / 2;
    long long divisor = static_cast<long long>(std::pow(10, halfDigits));
    long long secondHalf = number % divisor;
    long long firstHalf = number / divisor;

    return make_pair(firstHalf, secondHalf);
}


int main() {
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    //read input
    std::vector<long long> numbers;
    string line = lines[0];
    std::istringstream iss(line);
    long long num;
    while (iss >> num) {
        numbers.push_back(num);
    }
    map<long long, long long> frequency;
    for (auto x : numbers) {
        frequency[x]++;
    }
    // Process for 75 steps
    for (long long i = 0; i < 75; i++) {
        std::cout << "Step: " << i << "\n";
        map<long long, long long> temp;

        for(auto it = frequency.begin(); it != frequency.end(); it++){
            long long x = it->first;
            long long cnt = it->second;
            long long dig = numDigits(x);
            // Case 1
            if (x < 0) {
                std::cout << "Warning: Overflow!" << "\n";
            }
            if (x == 0) {
                temp[1] += cnt;
            }
            // Case 2
            else if (dig%2 == 0) {
                pair<long long, long long> split = splitEven(x);
                temp[split.first] += cnt;
                temp[split.second] += cnt;
            }
            // Case 3
            else {
                temp[x * 2024] += cnt;
            }
        }
        
        // Update map with results from this step
        frequency = temp;
    }

    long long ans = 0;
    for (auto it = frequency.begin(); it != frequency.end(); it++) {
        ans += it->second;
    }

    
    cout << ans << "\n";

    inFile.close();

    return 0;
}
