#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

struct Shape {
    vector<pair<int, int>> cells;
};

vector<Shape> getTransformations(const Shape& shape) {
    vector<Shape> result;
    set<vector<pair<int, int>>> seen;
    
    for (int flip = 0; flip < 2; flip++) {
        for (int rot = 0; rot < 4; rot++) {
            vector<pair<int, int>> cells = shape.cells;
            
            for (auto& p : cells) {
                int x = p.first, y = p.second;
                if (flip) y = -y;
                for (int r = 0; r < rot; r++) {
                    int tmp = x;
                    x = -y;
                    y = tmp;
                }
                p = {x, y};
            }
            
            int minX = 1e9, minY = 1e9;
            for (auto [x, y] : cells) {
                minX = min(minX, x);
                minY = min(minY, y);
            }
            for (auto& p : cells) {
                p.first -= minX;
                p.second -= minY;
            }
            
            sort(cells.begin(), cells.end());
            
            if (!seen.count(cells)) {
                seen.insert(cells);
                result.push_back({cells});
            }
        }
    }
    
    return result;
}

bool canPlace(const vector<vector<bool>>& grid, const Shape& shape, int x, int y) {
    int h = grid.size(), w = grid[0].size();
    for (auto [dx, dy] : shape.cells) {
        int nx = x + dx, ny = y + dy;
        if (nx < 0 || ny < 0 || nx >= w || ny >= h || grid[ny][nx])
            return false;
    }
    return true;
}

void place(vector<vector<bool>>& grid, const Shape& shape, int x, int y, bool val) {
    for (auto [dx, dy] : shape.cells) {
        grid[y + dy][x + dx] = val;
    }
}

bool solve(vector<vector<bool>>& grid, const vector<vector<Shape>>& trans, 
           vector<int>& counts) {
    
    bool allPlaced = true;
    for (int c : counts) if (c > 0) allPlaced = false;
    if (allPlaced) return true;
    
    int h = grid.size(), w = grid[0].size();
    
    for (int shapeIdx = 0; shapeIdx < counts.size(); shapeIdx++) {
        if (counts[shapeIdx] == 0) continue;
        
        for (const Shape& shape : trans[shapeIdx]) {
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    if (canPlace(grid, shape, x, y)) {
                        place(grid, shape, x, y, true);
                        counts[shapeIdx]--;
                        
                        if (solve(grid, trans, counts)) {
                            counts[shapeIdx]++;
                            place(grid, shape, x, y, false);
                            return true;
                        }
                        
                        counts[shapeIdx]++;
                        place(grid, shape, x, y, false);
                    }
                }
            }
        }
    }
    
    return false;
}

int main() {
    ifstream infile("day12.txt");
    if (!infile) {
        cerr << "Cannot open file\n";
        return 1;
    }

    string line;
    vector<Shape> baseShapes;
    
    for (int i = 0; i < 6; i++) {
        getline(infile, line);
        
        Shape shape;
        for (int row = 0; row < 3; row++) {
            getline(infile, line);
            for (int col = 0; col < line.size(); col++) {
                if (line[col] == '#') {
                    shape.cells.push_back({col, row});
                }
            }
        }
        baseShapes.push_back(shape);
        getline(infile, line);
    }
    
    vector<vector<Shape>> allTrans;
    for (const Shape& s : baseShapes) {
        allTrans.push_back(getTransformations(s));
    }
    
    int validRegions = 0;
    
    while (getline(infile, line)) {
        if (line.empty() || line.find('x') == string::npos) continue;
        
        size_t xPos = line.find('x');
        size_t colonPos = line.find(':');
        
        int w = stoi(line.substr(0, xPos));
        int h = stoi(line.substr(xPos + 1, colonPos - xPos - 1));
        
        vector<int> counts;
        istringstream iss(line.substr(colonPos + 1));
        int count;
        while (iss >> count) {
            counts.push_back(count);
        }
        
        int totalCells = 0;
        for (int i = 0; i < counts.size() && i < allTrans.size(); i++) {
            totalCells += counts[i] * allTrans[i][0].cells.size();
        }
        
        if (totalCells > w * h) continue;
        if (totalCells == 0) {
            validRegions++;
            continue;
        }
        
        vector<vector<bool>> grid(h, vector<bool>(w, false));
        if (solve(grid, allTrans, counts)) {
            validRegions++;
        }
    }
    
    cout << validRegions << endl;
    
    return 0;
}
