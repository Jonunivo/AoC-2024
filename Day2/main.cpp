#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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
        //std::cout << line << std::endl;
    }

    inFile.close();
    return lines;
}

bool check_safety(vector<int> numbers){
    bool safe = true;
    if(numbers.size() < 2){
        safe = false;
    }
    else if(numbers[0] == numbers[1]){
        safe = false;
    }
    else if(numbers[0] < numbers[1]){
        for(int i = 1; i<numbers.size(); i++){
            int prev = numbers[i-1];
            int curr = numbers[i];
            if(curr - prev < 1 || curr - prev > 3){
                safe = false;
                break;
            }
        }
    }
    else{
        for(int i = 1; i<numbers.size(); i++){
            int prev = numbers[i-1];
            int curr = numbers[i];
            if(prev - curr < 1 || prev - curr > 3){
                safe = false;
                break;
            }
        }
    }
    return safe;
}

int main() {
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> reports = readFileLineByLine(path);

    int counter = 0;
    for(auto it = reports.cbegin(); it != reports.cend(); it++){
        istringstream iss(*it);
        vector<int> numbers; 
        int num;
        while(iss >> num){
            numbers.push_back(num);
        }

        for(int i = 0; i<numbers.size(); i++){
            vector<int> newNumbers;
            for(int j = 0; j<numbers.size(); j++){
                if(i != j){
                    newNumbers.push_back(numbers[j]);
                }
            }
            if(check_safety(newNumbers)){
                counter++;
                cout << "SAFE" << "\n";
                break;
            }
            else{
                if(i == numbers.size()-1){
                    cout << "UNSAFE" << "\n";
                }   
                continue;
            }

        }

    }

    cout << counter;

    inFile.close();

    return 0;
}
