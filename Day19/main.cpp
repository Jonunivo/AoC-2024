// STL includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <set>
#include <sstream>
#include <limits>

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

int main()
{

    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    set<string> patterns;
    vector<string> designs;

    stringstream ss(lines[0]);
    string pattern;
    while (getline(ss, pattern, ',')) {
        // Trim whitespace around the pattern
        size_t start = pattern.find_first_not_of(" \t");
        size_t end = pattern.find_last_not_of(" \t");
        if (start != string::npos && end != string::npos) {
            patterns.insert(pattern.substr(start, end - start + 1));
        }
    }

    // Process the remaining lines for designs
    for (size_t i = 1; i < lines.size(); ++i) {
        if (!lines[i].empty()) {
            designs.push_back(lines[i]);
        }
    }

    int part1 = 0;
    long part2 = 0;

    for(string design : designs){
        int n = design.size();
        vector<long> dp(n + 1, 0);
        dp[0] = 1;

        for (int i = 1; i <= n; ++i) {
            for (const string& pattern : patterns) {
                int len = pattern.size();
                if (i >= len && design.substr(i - len, len) == pattern) {
                    dp[i] += dp[i - len];
                }
            }
        }

        part2 += dp[n];
        part1 += dp[n]==0 ? 0 : 1;
        //cout << dp[n] << "\n";
    }
    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";



    inFile.close();

    return 0;
}
