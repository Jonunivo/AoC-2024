#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <tuple>


using namespace std;

// Open file helper function
ifstream openFile(const string& fileName) {
    ifstream inFile(fileName);
    if (!inFile) {
        cerr << "Error: Could not open " << fileName << "\n";
    }
    return inFile;
}

// Read file line by line
vector<string> readFileLineByLine(const string& fileName) {
    ifstream inFile = openFile(fileName);
    string line;
    vector<string> lines;
    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();
    return lines;
}

int main() {
    const string fileName = "input"; // Specify the file name
    const string path = fileName + ".txt";
    ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    //read input
    vector<std::array<int, 5>> keys;
    vector<std::array<int, 5>> locks;
    size_t i = 0;
    size_t n = lines.size();

    while (i < n) {
        bool isLock = false;

        if (lines[i] == "#####") {
            //lock
            isLock = true;
        }
        i++;

        // Determine heights for this block
        int heights[5] = {0, 0, 0, 0, 0};

        for(int j = 0; j<5; j++) {
            const string& row = lines[i];
            for (int col = 0; col < 5; ++col) {
                if (row[col] == '#') {
                    heights[col]++;
                }
            }
            i++;
        }
        //skip last row
        i++;
        //skip empty row:
        i++;

        if(isLock){
            locks.push_back({heights[0], heights[1], heights[2], heights[3], heights[4]});
        }
        else{
            keys.push_back({heights[0], heights[1], heights[2], heights[3], heights[4]});
        }
    }
    
    //check every keys & locks pair
    int result = 0;
    for(auto it_l = locks.cbegin(); it_l!=locks.cend(); it_l++){
        for(auto it_k = keys.cbegin(); it_k!=keys.cend(); it_k++){
            bool overlap = false;
            for(int j = 0; j<5; j++){
                const int index = j;
                if ((*it_l)[j] + (*it_k)[j] > 5) {
                    overlap = true;
                    break;
                }
            }
            if(!overlap){
                result++;
            }
        }
    }

    cout << "Num of non-overlapping: " << result << "\n";


 
    inFile.close();
    return 0;
}
