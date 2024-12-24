#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>

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

int evaluateLogic(int val1, int val2, const string& operation) {
    if (operation == "AND") return val1 & val2;
    if (operation == "OR") return val1 | val2;
    if (operation == "XOR") return val1 ^ val2;
    return -1;
}

// Tests to find the incorrect registers.. (tests found on reddit)
vector<string> findIncorrectRegisters(
    const vector<tuple<string, string, string, string>>& operations) {

    struct Instr {
        string a, b, dst;
        string op; // XOR, AND, OR
    };

    vector<Instr> xyAdds;      // XYADD[n] above
    vector<Instr> xyCarries;   // XYCARRY[n] above
    vector<Instr> zOuts;       // These should be Zn outputs
    vector<Instr> ands;        // ANDS[$n] above
    vector<Instr> carries;     // CARRY[n] above

    for (const auto& [input1, input2, operation, output] : operations) {
        if (input1[0] == 'x' || input1[0] == 'y' || input2[0] == 'x' || input2[0] == 'y') {
            if (operation == "XOR") {
                xyAdds.push_back({input1, input2, output, operation});
            } else if (operation == "AND") {
                xyCarries.push_back({input1, input2, output, operation});
            }
            continue;
        }

        if (operation == "AND") {
            ands.push_back({input1, input2, output, operation});
        } else if (operation == "OR") {
            carries.push_back({input1, input2, output, operation});
        } else if (operation == "XOR") {
            zOuts.push_back({input1, input2, output, operation});
        }
    }

    vector<string> wrongs;

    // Check xyAdds
    for (auto& xy : xyAdds) {
        if (xy.a == "x00" || xy.a == "y00") // Special case, ignore
            continue;

        // If an XYAdd doesn't show up in XOR ops (or if it's a 'z' register), it's wrong
        if (xy.dst[0] == 'z' ||
            none_of(zOuts.begin(), zOuts.end(), [&](const auto& c) { return c.a == xy.dst || c.b == xy.dst; })) {
            wrongs.push_back(xy.dst);
        }
    }

    // Check xyCarries
    for (auto& xy : xyCarries) {
        if (xy.a == "x00" || xy.a == "y00") // Special case, ignore
            continue;

        // If an XYCarry doesn't show up in the carries (or if it's a 'z' register), it's wrong
        if (xy.dst[0] == 'z' ||
            none_of(carries.begin(), carries.end(), [&](const auto& c) { return c.a == xy.dst || c.b == xy.dst; })) {
            wrongs.push_back(xy.dst);
        }
    }

    // Check zOuts
    for (auto& v : zOuts) {
        if (v.dst[0] != 'z') {
            wrongs.push_back(v.dst);
        }
    }

    // Check carries
    string lastZ = "z" + to_string(zOuts.size() + 1);
    for (auto& v : carries) {
        if (v.dst[0] == 'z' && v.dst != lastZ) {
            wrongs.push_back(v.dst);
        }
    }

    // Check ands
    for (auto& v : ands) {
        if (v.dst[0] == 'z') {
            wrongs.push_back(v.dst);
        }
    }

    // Sort and remove duplicates
    sort(wrongs.begin(), wrongs.end());
    wrongs.erase(unique(wrongs.begin(), wrongs.end()), wrongs.end());

    return wrongs;
}


int main() {
    const string fileName = "input"; // Specify the file name
    const string path = fileName + ".txt";
    ifstream inFile = openFile(path);
    vector<string> lines = readFileLineByLine(path);

    //id -> value
    unordered_map<string, int> variableValues;
    //input_id,input_id,operation -> output_id
    vector<tuple<string, string, string, string>> operations;
    vector<tuple<string, string, string, string>> vis_operations;

    for(string line : lines){
        if (line.find(":") != string::npos) {
            stringstream ss(line);
            string id, valueStr;
            getline(ss, id, ':');
            getline(ss, valueStr);
            variableValues[id] = stoi(valueStr);
        } 
        else if (line.find("->") != string::npos) {
            stringstream ss(line);
            string input1, input2, operation, arrow, output;
            ss >> input1 >> operation >> input2 >> arrow >> output;
            operations.emplace_back(input1, input2, operation, output);
            vis_operations.emplace_back(input1, input2, operation, output);

            if (variableValues.find(input1) == variableValues.end()) variableValues[input1] = -1;
            if (variableValues.find(input2) == variableValues.end()) variableValues[input2] = -1;
            if (variableValues.find(output) == variableValues.end()) variableValues[output] = -1;

        }
    }

    //PART 1
    while (!operations.empty()) {
        vector<tuple<string, string, string, string>> remainingOperations; // Hold operations that can't be processed yet

        for (const auto& [input1, input2, operation, output] : operations) {
            // Check if inputs are ready
            if (variableValues[input1] == -1 || variableValues[input2] == -1) {
                remainingOperations.push_back({input1, input2, operation, output});
            } else {
                // Both inputs are ready; perform the operation
                variableValues[output] = evaluateLogic(variableValues[input1], variableValues[input2], operation);
            }
        }

        // Update the operations list
        operations = move(remainingOperations);
    }

    //Print z variables
    vector<pair<string, int>> zVariables;
    for (const auto& [id, value] : variableValues) {
        if (id[0] == 'z') {
            zVariables.emplace_back(id, value);
        }
    }
    sort(zVariables.rbegin(), zVariables.rend());
    cout << "Variables starting with 'z':" << endl;
    for (const auto& [id, value] : zVariables) {
        cout << id << ": " << value << endl;
    }
    string binaryNumber;
    for (const auto& [id, value] : zVariables) {
        binaryNumber += to_string(value);
    }

    //Graphviz
    ofstream dotFile("graph.dot");
    if (!dotFile) {
        cerr << "Error creating graph.dot file." << endl;
        return 1;
    }

    dotFile << "digraph LogicCircuit {" << endl;
    dotFile << "  rankdir=LR;" << endl; // Left-to-right layout

    // Define nodes with variable values
    for (const auto& [id, value] : variableValues) {
        string color = (id[0] == 'z') ? "red" : "black"; // Highlight 'z' variables in red
        string label = id + " = " + to_string(value);
        dotFile << "  " << id << " [label=\"" << label << "\", color=" << color << "];" << endl;
    }

    // Define edges for operations
    for (const auto& [input1, input2, operation, output] : vis_operations) {
        dotFile << "  " << input1 << " -> " << output << " [label=\"" << operation << "\"];" << endl;
        dotFile << "  " << input2 << " -> " << output << " [label=\"" << operation << "\"];" << endl;
    }
    dotFile << "}" << endl;
    dotFile.close();


    //PART 2: Find incorrect registers
    vector<string> incorrectRegisters = findIncorrectRegisters(vis_operations);

    // Output results
    cout << "+————————————————————————————————————————————————————————————————————————————————————————————————————+" << endl;

    long long decimalNumber = stoll(binaryNumber, nullptr, 2);
    cout << "| Binary number: " << binaryNumber << "                                      |" << endl;
    cout << "| Decimal number: " << decimalNumber << "                                                                     |" <<endl;
    cout << "| Graph written to graph.dot. Use Graphviz to visualize it (e.g., dot -Tpng graph.dot -o graph.png). |" << endl;


    cout << "| Incorrect Registers: ";
    for (size_t i = 0; i < incorrectRegisters.size(); ++i) {
        cout << incorrectRegisters[i];
        if (i < incorrectRegisters.size() - 1) {
            cout << ",";
        }
    }
    cout << "                                               |" << endl;

    cout << "+————————————————————————————————————————————————————————————————————————————————————————————————————+" << endl;

 
    inFile.close();
    return 0;
}
