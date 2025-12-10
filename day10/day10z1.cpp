#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <limits>

using namespace std;

struct Machine {
    vector<bool> target;
    vector<vector<int>> buttons;
};

Machine parseLine(const string& line) {
    Machine m;
    
    size_t start = line.find('[');
    size_t end = line.find(']');
    string target_str = line.substr(start + 1, end - start - 1);
    
    for (char c : target_str) {
        m.target.push_back(c == '#');
    }
    
    regex button_regex(R"(\(([0-9,]+)\))");
    auto buttons_begin = sregex_iterator(line.begin(), line.end(), button_regex);
    auto buttons_end = sregex_iterator();
    
    for (auto it = buttons_begin; it != buttons_end; ++it) {
        vector<int> button;
        string button_str = (*it)[1].str();
        
        stringstream ss(button_str);
        string num;
        while (getline(ss, num, ',')) {
            button.push_back(stoi(num));
        }
        m.buttons.push_back(button);
    }
    
    return m;
}

int solveMachine(const Machine& m) {
    int n_lights = m.target.size();
    int n_buttons = m.buttons.size();
    
    vector<vector<int>> matrix(n_lights, vector<int>(n_buttons + 1, 0));
    
    for (int j = 0; j < n_buttons; j++) {
        for (int light : m.buttons[j]) {
            if (light < n_lights) {
                matrix[light][j] = 1;
            }
        }
    }
    
    for (int i = 0; i < n_lights; i++) {
        matrix[i][n_buttons] = m.target[i] ? 1 : 0;
    }
    
    vector<int> pivot_col(n_lights, -1);
    int rank = 0;
    
    for (int col = 0; col < n_buttons && rank < n_lights; col++) {
        int pivot_row = -1;
        for (int row = rank; row < n_lights; row++) {
            if (matrix[row][col] == 1) {
                pivot_row = row;
                break;
            }
        }
        
        if (pivot_row == -1) continue;
        
        if (pivot_row != rank) {
            swap(matrix[pivot_row], matrix[rank]);
        }
        
        pivot_col[rank] = col;
        
        for (int row = 0; row < n_lights; row++) {
            if (row != rank && matrix[row][col] == 1) {
                for (int c = 0; c <= n_buttons; c++) {
                    matrix[row][c] ^= matrix[rank][c];
                }
            }
        }
        
        rank++;
    }
    
    for (int row = rank; row < n_lights; row++) {
        if (matrix[row][n_buttons] == 1) {
            return -1;
        }
    }
    
    vector<bool> is_pivot(n_buttons, false);
    for (int i = 0; i < rank; i++) {
        if (pivot_col[i] != -1) {
            is_pivot[pivot_col[i]] = true;
        }
    }
    
    vector<int> free_vars;
    for (int i = 0; i < n_buttons; i++) {
        if (!is_pivot[i]) {
            free_vars.push_back(i);
        }
    }
    
    int min_presses = INT_MAX;
    int num_free = free_vars.size();
    
    for (int mask = 0; mask < (1 << num_free); mask++) {
        vector<int> solution(n_buttons, 0);
        
        for (int i = 0; i < num_free; i++) {
            solution[free_vars[i]] = (mask >> i) & 1;
        }
        
        for (int i = rank - 1; i >= 0; i--) {
            if (pivot_col[i] != -1) {
                int val = matrix[i][n_buttons];
                for (int j = 0; j < n_buttons; j++) {
                    if (j != pivot_col[i] && matrix[i][j] == 1) {
                        val ^= solution[j];
                    }
                }
                solution[pivot_col[i]] = val;
            }
        }
        
        vector<int> result(n_lights, 0);
        for (int j = 0; j < n_buttons; j++) {
            if (solution[j] == 1) {
                for (int light : m.buttons[j]) {
                    if (light < n_lights) {
                        result[light] ^= 1;
                    }
                }
            }
        }
        
        bool valid = true;
        for (int i = 0; i < n_lights; i++) {
            if (result[i] != (m.target[i] ? 1 : 0)) {
                valid = false;
                break;
            }
        }
        
        if (valid) {
            int presses = 0;
            for (int x : solution) {
                presses += x;
            }
            min_presses = min(min_presses, presses);
        }
    }
    
    return (min_presses == INT_MAX) ? -1 : min_presses;
}

int main(int argc, char* argv[]) {
    string filename = "day10.txt";
    if (argc > 1) {
        filename = argv[1];
    }
    
    ifstream infile(filename);
    if (!infile) {
        cerr << "Could not open file '" << filename << "'\n";
        return 1;
    }
    
    int total_presses = 0;
    string line;
    int machine_num = 0;
    
    while (getline(infile, line)) {
        if (line.empty()) continue;
        
        machine_num++;
        Machine m = parseLine(line);
        
        int presses = solveMachine(m);
        
        if (presses == -1) {
            cout << "Machine " << machine_num << ": No solution" << endl;
        } else {
            cout << "Machine " << machine_num << ": " << presses << " presses" << endl;
            total_presses += presses;
        }
    }
    
    cout << "\nTotal button presses needed: " << total_presses << endl;
    
    return 0;
}
