#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

int main() {
    std::ifstream infile("day6z1.txt");
    if (!infile) {
        std::cerr << "Could not open file 'day6z1.txt'" << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    size_t maxLen = 0;
    while (std::getline(infile, line)) {
        lines.push_back(line);
        maxLen = std::max(maxLen, line.length());
    }
    infile.close();

    for (auto& l : lines) {
        if (l.length() < maxLen) {
            l.resize(maxLen, ' ');
        }
    }

    if (lines.size() < 2) return 0;

    int numRows = lines.size();
    int numCols = maxLen;
    std::string opLine = lines.back();

    long long grandTotal = 0;

    int startCol = 0;
    while (startCol < numCols) {
        bool isEmpty = true;
        for (int r = 0; r < numRows; r++) {
            if (lines[r][startCol] != ' ') {
                isEmpty = false;
                break;
            }
        }
        if (isEmpty) {
            startCol++;
            continue;
        }

        int endCol = startCol;
        while (endCol < numCols) {
            bool colEmpty = true;
            for (int r = 0; r < numRows; r++) {
                if (lines[r][endCol] != ' ') {
                    colEmpty = false;
                    break;
                }
            }
            if (colEmpty) break;
            endCol++;
        }
        
        int blockWidth = endCol - startCol;
        
        char op = ' ';
        for (int c = startCol; c < endCol; c++) {
            if (opLine[c] != ' ') {
                op = opLine[c];
                break;
            }
        }

        if (op == ' ') {
            startCol = endCol;
            continue;
        }

        std::vector<long long> numbers;
        for (int c = startCol; c < endCol; c++) {
            std::string numStr = "";
            for (int r = 0; r < numRows - 1; r++) {
                char ch = lines[r][c];
                if (std::isdigit(ch)) {
                    numStr += ch;
                }
            }
            
            if (!numStr.empty()) {
                long long num = std::stoll(numStr);
                numbers.push_back(num);
                std::cout << "Column " << (c - startCol) << ": " << num << std::endl;
            }
        }

        if (!numbers.empty()) {
            long long result = numbers[0]; 
            for (size_t i = 1; i < numbers.size(); i++) {
                if (op == '+') result += numbers[i];
                else if (op == '*') result *= numbers[i];
            }
            std::cout << "Block result (" << op << "): " << result << std::endl;
            grandTotal += result;
        }

        startCol = endCol;
    }

    std::cout << "Grand total: " << grandTotal << std::endl;

    return 0;
}