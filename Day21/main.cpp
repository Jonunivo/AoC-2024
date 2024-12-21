// STL includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <deque>
#include <map>
#include <cmath>
#include <limits>
#include <sstream>

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

int64_t cheapestRobot(const std::string& presses, int nrobots);


uint64_t customHash(int curr, int curc, int destr, int destc, int nrobots)
{
    uint64_t result = curr;
    result *= 4;
    result += curc;
    result *= 4;
    result += destr;
    result *= 4;
    result += destc;
    result *= 30;
    result += nrobots;
    return result;
}

std::map<uint64_t, int64_t> memo;


int64_t cheapestDirPad(int curr_row, int curr_col, int dest_row, int dest_col, int nrobots){
    uint64_t h = customHash(curr_row, curr_col, dest_row, dest_col, nrobots);
    auto it = memo.find(h);
    if(it != memo.end()){
        return it->second;
    }

    int64_t answer = numeric_limits<int64_t>::max();
    struct Visit{
        int row, col;
        string presses;
    };
    deque<Visit> q;
    q.push_back({curr_row, curr_col, ""});
    while(!q.empty()){
        auto v = q.front();
        q.pop_front();
        if(v.row == dest_row && v.col == dest_col){
            int64_t rec = cheapestRobot(v.presses + "A", nrobots-1);
            answer = min(answer, rec);
            continue;
        }
        if(v.row == 0 && v.col == 0){
            continue;
        }
        else{
            if(v.row < dest_row){
                q.push_back({v.row + 1, v.col, v.presses + "v"});
            }
            else if(v.row > dest_row){
                q.push_back({v.row - 1, v.col, v.presses + "^"});
            }

            if(v.col < dest_col){
                q.push_back({ v.row, v.col + 1, v.presses + ">" });
            }
            else if(v.col > dest_col){
                q.push_back({ v.row, v.col - 1, v.presses + "<" });
            }
        }
    }
    memo[h] = answer;
    return answer;
}

int64_t cheapestRobot(const string &presses, int nrobots){
    if(nrobots == 1){
        return presses.length();
    }

    int64_t result = 0;
    string padConfig = "X^A<v>";
    
    int curr_row = 0;
    int curr_col = 2;
    for(int i = 0; i<presses.length(); i++){
        for(int next_row = 0; next_row < 2; next_row++){
            for(int next_col = 0; next_col < 3; next_col++){
                if(padConfig[next_row*3 + next_col] == presses[i]){
                    result += cheapestDirPad(curr_row, curr_col, next_row, next_col, nrobots);
                    curr_row = next_row;
                    curr_col = next_col;
                }
            }
        }
    }
    return result;
}

int64_t cheapest(int curr_row, int curr_col, int dest_row, int dest_col){
    int64_t answer = numeric_limits<int64_t>::max();
    struct Visit{
        int row, col;
        string presses;
    };

    deque<Visit> q;
    q.push_back({curr_row, curr_col, ""});
    while(!q.empty()){
        auto v = q.front();
        q.pop_front();
        if(v.row == dest_row && v.col == dest_col){
            int64_t rec = cheapestRobot(v.presses + "A", 26);
            answer = min(answer, rec);
            continue;
        }
        if(v.row == 3 && v.col == 0){
            continue;
        }
        else{
            if (v.row < dest_row)
            {
                q.push_back({ v.row + 1,v.col,v.presses + "v" });
            }
            else if (v.row > dest_row)
            {                
                q.push_back({ v.row - 1, v.col, v.presses + "^" });
            }
            if (v.col < dest_col)
            {
                q.push_back({ v.row, v.col + 1, v.presses + ">" });
            }
            else if (v.col > dest_col)
            {
                q.push_back({ v.row, v.col - 1, v.presses + "<" });
            }
        }
    }
    return answer;

}

int main(){

    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    int64_t sum = 0;
    for(string line : lines){
        int64_t result = 0;
        string padConfig = "789456123X0A";
        int curr_row = 3;
        int curr_col = 2;
        for(int i = 0; i<line.length(); i++){
            for(int next_row = 0; next_row < 4; next_row++){
                for(int next_col = 0; next_col < 3; next_col++){
                    if(padConfig[next_row*3 + next_col] == line[i]){
                        result += cheapest(curr_row, curr_col, next_row, next_col);
                        curr_row = next_row;
                        curr_col = next_col;
                    }
                }
            }
        }

        stringstream ss(line);
        int code; ss >> code;
        sum += result * code;

        cout << "Input: " << line << "\n";
        cout << "shortest seq: " << result << "\n";
        cout << "code: " << code << "\n";
    }

    cout << "Final Result: " << sum << "\n";

    

    inFile.close();

    return 0;
}
