#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

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

void printState(vector<int> diskMap){
    //debug printing
    for(int i = 0; i<diskMap.size(); i++){
        if(diskMap[i] == -1){
            cout << ".";
        }
        else{
            cout << diskMap[i];
        }
    }
    cout << "\n";
}

int main() {
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    //Step 1: Input -> Disk Map
    vector<int> diskMap;
    string input = lines[0];
    int id = 0;
    int pos = 0;
    for(int i = 0; i<input.length(); i++){
        //occupied space
        int len = input[i] - '0';
        for(int j = 0; j<len; j++){
            diskMap.push_back(id);
        }
        id++;
        //free space
        if(++i < input.length()){
            int freelen = input[i] - '0';
            for(int j = 0; j<freelen; j++){
                diskMap.push_back(-1);
            }
        }
    }

    //Step 2: Move blocks from back to front // Part 1
    /*
    int left = 0;
    int right = diskMap.size()-1;
    while(left < right){
        //move into position
        while(diskMap[left] != -1){
            left++;
        }
        while(diskMap[right] == -1){
            right--;
        }
        //swap
        if(left < right){
            diskMap[left] = diskMap[right];
            diskMap[right] = -1;
        }
    }
    */
    //Step 2: Move blocks from back to front // Part 2
    int left = 0;
    int right = diskMap.size()-1;

    //Find File Size
    while(left < right){
        //find & measure file size
        while(diskMap[right] == -1){
            right--;
            //cout << ".";            
        }
        int fileIter = right;
        int fileSize = 0;
        int fileId = diskMap[right];
        while(diskMap[fileIter] == fileId){
            fileSize++;
            fileIter--;
            //cout << "|"; 
        }
        right = fileIter;

        while(left < right){
            //find big enough gap
            while(diskMap[left] != -1){
                left++;
                //cout << "#"; 
            }
            int gapSize = 0;
            int gapIter = left;
            while(diskMap[gapIter] == -1){
                gapIter++;
                gapSize++;
                //cout << "="; 
            }
            if(gapSize >= fileSize){
                //found big enough space: swap
                int fillGapIter = left;
                int takeSizeIter = fileIter + 1;
                for(int i = 0; i<fileSize; i++){
                    diskMap[fillGapIter + i] = diskMap[takeSizeIter + i];
                    diskMap[takeSizeIter + i] = -1;
                    //cout << i << "|" << fileId << ": ";
                    //printState(diskMap);

                }
                break;
            }
            else{
                //space not big enough, find next:
                left = gapIter;
            }
        }
        //Put left back to first gap
        left = 0;
        while(diskMap[left] != -1){
                left++;
                //cout << "#"; 
        }

       // printState(diskMap);
        
    
    }   

    printState(diskMap);
    //Step 3: output
    long result = 0;
    for(int i = 0; i<diskMap.size(); i++){
        if(diskMap[i] != -1){
            result += diskMap[i]*i;
        }
    }

    cout << "\n" << "result: " << result << "\n";



    inFile.close();

    return 0;
}
