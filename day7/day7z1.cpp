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

struct Beam {
    int row;
    int col;
};

int main()
{
    std::ifstream infile("day7.txt");
    if (!infile)
    {
        std::cerr << "Could not open file 'day7.txt'\n";
        return 1;
    }

    std::vector<std::string> grid;
    std::string line;
    while (std::getline(infile, line))
    {
        grid.push_back(line);
    }
    infile.close();

    int rows = grid.size();
    int cols = grid[0].size();
    
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
    
    std::queue<Beam> q;
    std::set<std::pair<int, int>> visited;
    std::set<std::pair<int, int>> splittersWithBeamAbove;
    
    q.push({startRow, startCol});
    visited.insert({startRow, startCol});
    
    while (!q.empty()) {
        Beam beam = q.front();
        q.pop();
        
        int r = beam.row;
        int c = beam.col;
        
        while (true) {
            r++;
            
            if (r < 0 || r >= rows || c < 0 || c >= cols) break;
            
            char cell = grid[r][c];
            
            if (cell == '^') {
                splittersWithBeamAbove.insert({r, c});
                
                auto leftState = std::make_pair(r, c - 1);
                if (visited.find(leftState) == visited.end()) {
                    visited.insert(leftState);
                    q.push({r, c - 1});
                }
                
                auto rightState = std::make_pair(r, c + 1);
                if (visited.find(rightState) == visited.end()) {
                    visited.insert(rightState);
                    q.push({r, c + 1});
                }
                
                break;
            }
        }
    }
    
    int splitCount = splittersWithBeamAbove.size();

    std::cout << splitCount << std::endl;

    return 0;
}