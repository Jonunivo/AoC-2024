#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
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
    }

    inFile.close();
    return lines;
}

int main() {
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    unordered_map<int, vector<int>> rules;
    vector<vector<int>> updates;

    int i = 0;
    while (i < lines.size() && !lines[i].empty()) {
        const std::string &line = lines[i];
        size_t pos = line.find('|');
        if (pos != std::string::npos) {
            int key = std::stoi(line.substr(0, pos));
            int value = std::stoi(line.substr(pos + 1));
            if(rules.count(key) == 0){
                vector<int> curr = {value};
                rules[key] = curr;
            }
            else{
                rules[key].push_back(value);
            }
        }
        i++;
    }
    i++;
    while (i < lines.size() && !lines[i].empty()) {
        const std::string &line = lines[i];
        std::vector<int> tempVector;
        std::istringstream ss(line);
        std::string num;
        while (std::getline(ss, num, ',')) {
            tempVector.push_back(std::stoi(num));
        }
        updates.push_back(tempVector);
        i++;
    }

    //check for each update if it is valid & retun middle number
    int result = 0;
    for(auto it = updates.cbegin(); it != updates.cend(); it++){
        bool valid = true;
        set<int> numbers_before;
        for(int i = it->size()-2; i >= 0; i--){
            numbers_before.insert(it->at(i));
        }
        //check for each key if any value exists
        for(int i = it->size()-1; i >= 0; i--){
            vector<int> curr_rules = rules[it->at(i)];
            for(auto it2 = curr_rules.cbegin(); it2!=curr_rules.cend(); it2++){
                if(numbers_before.count(*it2) > 0){
                    //rule break!
                    //swap & restart
                    valid = false;
                    break;
                }
            }
            numbers_before.erase(it->at(i));
        }
        if(valid){
            int mid_value = it->at(it->size() / 2);
            result += mid_value;
        }
    }

    cout << result;
    inFile.close();

    return 0;
}
