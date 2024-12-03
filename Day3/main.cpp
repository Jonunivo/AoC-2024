#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <regex>


using namespace std;

// Function to open a file and return the input file stream
std::ifstream openFile(const std::string& fileName) {
    std::ifstream inFile(fileName);
    if (!inFile) {
        std::cerr << "Error: Could not open " << fileName << "\n";
    }
    return inFile;
}

int main() {
    const std::string fileName = "input"; // Specify the file name
    const std::string path = "C:/Users/uniho/OneDrive/Documents/AoC-2024/Day3/" + fileName + ".txt";
    std::ifstream inFile = openFile(path);

    string input((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    // Define a combined regular expression for all relevant instructions
    std::regex instruction_regex(R"((do\(\))|(don't\(\))|(mul\((\d{1,3}),(\d{1,3})\)))");

    std::smatch match;
    auto begin = input.cbegin();
    auto end = input.cend();

    int total = 0;
    bool mul_enabled = true; 

    while (std::regex_search(begin, end, match, instruction_regex)) {
        if (match[1].matched) { // "do()" matched
            mul_enabled = true;
        } else if (match[2].matched) { // "don't()" matched
            mul_enabled = false;
        } else if (match[3].matched && mul_enabled) { // "mul(X,Y)" matched and enabled
            int num1 = std::stoi(match[4].str());
            int num2 = std::stoi(match[5].str());
            total += num1 * num2;
        }

        // Move the search start position forward
        begin = match.suffix().first;
    }

    // Output the result
    std::cout << "Total sum of all valid mul results: " << total << std::endl;

}
