// STL includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <sstream>
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
        //std::cout << line << std::endl;
    }

    inFile.close();
    return lines;
}

//opCode 0
void adv(int& A, int& B, int combo){
    int denominator = 1 << combo;
    A = A / denominator;
}
//opCode 1
void bxl(int& B, int literal){
    B = B ^ literal;
}
//opCode 2
void bst(int& B, int combo){
    B = combo%8;
}
//opCode 3
void jnz(int& A, int& instruction_pointer, int literal){
    if(A != 0){
        instruction_pointer = literal - 2;
    }
}
//opCode 4
void bxc(int& B, int& C, int operand){
    B = B ^ C;
}
//opCode 5
void out(int combo){
    //cout << combo%8 << ",";
}
//opCode 6
void bdv(int& A, int& B, int combo){
    int denominator = 1 << combo;
    B = A / denominator;
}
//opCode 7
void cdv(int& A, int& C, int combo){
    int denominator = 1 << combo;
    C = A / denominator;
}

//combo
int calcCombo(int literal, int& A, int& B, int& C) {
    switch(literal) {
        case 0:
        case 1:
        case 2:
        case 3:
            return literal;
        case 4:
            return A;
        case 5:
            return B;
        case 6:
            return C; 
        case 7:
            return -1;
        default:
            return -1;
    }
}

vector<int> simulateProgram(long initialA, const vector<int>& program) {
    int A = initialA;
    int B = 0;
    int C = 0;
    int instruction_pointer = 0;
    vector<int> output;

    while (instruction_pointer < program.size() - 1) {
        int literal = program[instruction_pointer + 1];
        int combo = calcCombo(literal, A, B, C);
        switch (program[instruction_pointer]) {
            case 0: adv(A, B, combo); break;
            case 1: bxl(B, literal); break;
            case 2: bst(B, combo); break;
            case 3: jnz(A, instruction_pointer, literal); break;
            case 4: bxc(B, C, literal); break;
            case 5: out(combo); output.push_back(combo % 8); break;
            case 6: bdv(A, B, combo); break;
            case 7: cdv(A, C, combo); break;
        }
        instruction_pointer += 2;
    }
    return output;
}

int findInitialA(const vector<int>& program) {
    int lowestA = pow(2,15);
    while (true) {
        vector<int> output = simulateProgram(lowestA, program);

        if (output == vector<int>(program.begin(), program.end())) {
            return lowestA;
        }
        // Increment and try the next value for register A
        lowestA++;

        if(lowestA % 100000 == 0){
            cout << lowestA << "\n";
        }
    }
}

int main()
{
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);
    
    int instruction_pointer = 0;
    int A, B, C;
    vector<int> program;

    //Read Input
    istringstream registerA(lines[0]);
    string temp;
    registerA >> temp >> temp; 
    registerA >> A;
    istringstream registerB(lines[1]);
    registerB >> temp >> temp; 
    registerB >> B;
    istringstream registerC(lines[2]);
    registerC >> temp >> temp; 
    registerC >> C;
    istringstream programLine(lines[4].substr(9)); 
    int value;
    while (programLine >> value) {
        program.push_back(value);
        if (programLine.peek() == ',') programLine.ignore(); // Ignore commas
    }

    //run program
    int correctA = findInitialA(program);
    cout << "The lowest initial value for register A that outputs the program is: " << correctA << endl;







    return 0;
}
