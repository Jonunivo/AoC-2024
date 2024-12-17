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
void adv(long long& A, long long& B, long long combo){
    long long denominator = 1 << combo;
    A = A / denominator;
}
//opCode 1
void bxl(long long& B, long long literal){
    B = B ^ literal;
}
//opCode 2
void bst(long long& B, long long combo){
    B = combo%8;
}
//opCode 3
void jnz(long long& A, long long& instruction_pointer, long long literal){
    if(A != 0){
        instruction_pointer = literal - 2;
    }
}
//opCode 4
void bxc(long long& B, long long& C, long long operand){
    B = B ^ C;
}
//opCode 5
void out(long long combo){
    //cout << combo%8 << ",";
}
//opCode 6
void bdv(long long& A, long long& B, long long combo){
    long long denominator = 1 << combo;
    B = A / denominator;
}
//opCode 7
void cdv(long long& A, long long& C, long long combo){
    long long denominator = 1 << combo;
    C = A / denominator;
}

//combo
long long calcCombo(long long literal, long long& A, long long& B, long long& C) {
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

vector<long long> simulateProgram(long initialA, const vector<long long>& program) {
    long long A = initialA;
    long long B = 0;
    long long C = 0;
    long long instruction_pointer = 0;
    vector<long long> output;

    while (instruction_pointer < program.size() - 1) {
        long long literal = program[instruction_pointer + 1];
        long long combo = calcCombo(literal, A, B, C);
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

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

long long numMatch(const std::vector<long long> &pred, const std::vector<long long> &gold) {
  long long i{static_cast<long long>(gold.size() - 1)};
  long long j{static_cast<long long>(pred.size() - 1)};
  long long num{0};
  while (i >= 0 && j >= 0) {
    if (gold[i] != pred[j]) {
      return num;
    }
    ++num;
    --j;
    --i;
  }
  return num;
}

long long findInitialA(const vector<long long>& program) {
    long a{0};
    long long a7{0};
    long long n{0};

    while (true) {
        vector<long long> output = simulateProgram(a, program);

        int nMatch{numMatch(output, program)};
        if(nMatch > n){

            // Debugging prints
            cout << "Current A: " << a << "\n";
            cout << "Output:    ";
            for (int i = output.size() - 1; i >= 0; --i) {
                cout << output[i] << " ";
            }
            cout << "\n";
            cout << "Program:   ";
            for (int i = program.size() - 1; i >= 0; --i) {
                cout << program[i] << " ";
            }
            cout << "\n";
            cout << "nMatch:    " << nMatch << "\n";
            cout << "-------------------------------------------"  << "\n";

            if(nMatch == 16){
                return a;
            }
            n = nMatch;
            a <<= 3;
        }
        else if(a7 == 7){
            //not found, backtrack!
            a >>= 3;
            ++a;
            --n;
        }
        else{
            //keep searching
            ++a;
        }
        a7 = a & 7;
    }
}


int main()
{
    const std::string fileName = "input"; // Specify the file name
    const std::string path = fileName + ".txt";
    std::ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);
    
    long long instruction_pointer = 0;
    long long A, B, C;
    vector<long long> program;

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
    long long value;
    while (programLine >> value) {
        program.push_back(value);
        if (programLine.peek() == ',') programLine.ignore(); // Ignore commas
    }

    //run program
    long long correctA = findInitialA(program);
    cout << "The lowest initial value for register A that outputs the program is: " << correctA << endl;







    return 0;
}
