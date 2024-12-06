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

bool swapInvalid(vector<int> &update, unordered_map<int, vector<int>> rules){
        // Iterate through the update vector from back to front
    bool found_invalid =false;
    for (size_t i = update.size() - 1; i > 0; --i) {
        int key = update[i];
        for (int j = i-1; j >= 0; --j) {
            int query = update[j];
            for(int k = 0; k<rules[key].size(); k++){
                if(rules[key][k] == query){
                    //swap
                    found_invalid =true;
                    int old_j = update[j];
                    update[j] = update[i];
                    update[i] = old_j;
                }
            }
        }
    }
    return found_invalid;
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
    for(auto it = updates.begin(); it != updates.end(); it++){
        bool invalid = false;
        bool once_invalid = false;
        do{
            invalid = swapInvalid(*it, rules);
            if(invalid){
                once_invalid = true;
            }
        }while(invalid);

        
        if(once_invalid){

            int mid_value = it->at(it->size() / 2);
            result += mid_value;
        }
    }

    cout << result;
    inFile.close();

    return 0;
}
