#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <map>
#include <set>
#include <queue>
#include <algorithm>

std::vector<std::string> grid;
int rows, cols;
std::map<std::pair<int, int>, long long> memo;

long long countTimelines(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return 1;
    }
    
    auto key = std::make_pair(row, col);
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }
    
    char cell = grid[row][col];
    long long result;
    
    if (cell == '^') {
        long long leftTimelines = countTimelines(row + 1, col - 1);
        long long rightTimelines = countTimelines(row + 1, col + 1);
        result = leftTimelines + rightTimelines;
    } else {
        result = countTimelines(row + 1, col);
    }
    
    memo[key] = result;
    return result;
}

int main()
{
    std::ifstream infile("day7.txt");
    if (!infile)
    {
        std::cerr << "Could not open file 'day7.txt'\n";
        return 1;
    }

    std::string line;
    while (std::getline(infile, line))
    {
        grid.push_back(line);
    }
    infile.close();

    rows = grid.size();
    cols = grid[0].size();
    
    int startRow = -1, startCol = -1;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 'S') {
                startRow = r;
                startCol = c;
                break;
            }
        }
        if (startRow != -1) break;
    }
    
    long long totalTimelines = countTimelines(startRow + 1, startCol);
    
    std::cout << totalTimelines << std::endl;

    return 0;
}