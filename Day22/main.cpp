#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <tuple>
#include <unordered_map>

using namespace std;

namespace std {
    template <>
    struct hash<std::tuple<int, int, int, int>> {
        size_t operator()(const std::tuple<int, int, int, int>& t) const {
            size_t h1 = std::hash<int>{}(std::get<0>(t));
            size_t h2 = std::hash<int>{}(std::get<1>(t));
            size_t h3 = std::hash<int>{}(std::get<2>(t));
            size_t h4 = std::hash<int>{}(std::get<3>(t));

            // Combine the hashes in a way that minimizes collisions
            size_t combined_hash = h1 ^ h2 ^ h3 ^ h4; // XOR all hashes
            return combined_hash;
        }
    };
}

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

// Mixing and pruning function
long mixAndPrune(long old_result, long result) {
    long res = old_result ^ result;
    res = res % 16777216;
    return res;
}

int main() {
    const string fileName = "input"; // Specify the file name
    const string path = fileName + ".txt";
    ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    vector<long> numbers;
    for (const string& line : lines) {
        numbers.push_back(stol(line));
    }

    int n = 2000;
    vector<vector<int>> sequences;
    vector<tuple<int, int, int, int>> allGoodTuples;
    vector<vector<int>> prices;

    vector<unordered_map<std::tuple<int, int, int, int>, int>> tupleMaps;
    unordered_map<std::tuple<int, int, int, int>, int> globalTuplePriceSum;

    // Process each number
    int k = 0;
    for (long number : numbers) {
        int prev_last = number % 10;
        vector<int> sequence(n);
        vector<int> price(n);
        long result = number;

        std::unordered_map<std::tuple<int, int, int, int>, int> tupleMap;

        for (int i = 0; i < n; i++) {
            long old_result = result;
            result *= 64;
            result = mixAndPrune(old_result, result);
            old_result = result;
            result /= 32;
            result = mixAndPrune(old_result, result);
            old_result = result;
            result *= 2048;
            result = mixAndPrune(old_result, result);
            price[i] = result % 10;
            sequence[i] = (result % 10) - prev_last;
            prev_last = result % 10;

            if (i >= 3) {
                std::tuple<int, int, int, int> currentTuple = {sequence[i-3], sequence[i-2], sequence[i-1], sequence[i]};
                if (tupleMap.find(currentTuple) == tupleMap.end()) {
                    tupleMap[currentTuple] = price[i];
                }
            }
        }

        // Add to the sequences and prices
        sequences.push_back(sequence);
        prices.push_back(price);
        tupleMaps.push_back(tupleMap);

        std::cout << "done " << k << " out of " << 2000 << "\n";
        k++;

        // Add to the global map: Sum prices for each tuple across all tupleMaps
        for (const auto& entry : tupleMap) {
            const auto& currentTuple = entry.first;
            int currentPrice = entry.second;

            globalTuplePriceSum[currentTuple] += currentPrice; // Sum prices for each tuple
        }
    }

    // Now, find the tuple with the highest sum of prices across all tupleMaps
    tuple<int, int, int, int> bestTuple;
    int maxPriceSum = 0;

    for (const auto& entry : globalTuplePriceSum) {
        const auto& currentTuple = entry.first;
        int currentPriceSum = entry.second;

        if (currentPriceSum > maxPriceSum) {
            bestTuple = currentTuple;
            maxPriceSum = currentPriceSum;
        }
    }

    // Output the best tuple and the corresponding sum of prices
    std::cout << "Best tuple: ("
         << get<0>(bestTuple) << ", "
         << get<1>(bestTuple) << ", "
         << get<2>(bestTuple) << ", "
         << get<3>(bestTuple) << ") -> NumBananas: " << maxPriceSum << "\n";


    inFile.close();
    return 0;
}
