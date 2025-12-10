#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <limits>
#include <cmath>

using namespace std;

struct Machine {
    vector<long long> target_joltage;
    vector<vector<int>> buttons;
};

Machine parseLine(const string& line) {
    Machine m;
    
    size_t start = line.find('{');
    size_t end = line.find('}');
    string joltage_str = line.substr(start + 1, end - start - 1);
    
    stringstream ss_jolt(joltage_str);
    string num;
    while (getline(ss_jolt, num, ',')) {
        m.target_joltage.push_back(stoll(num));
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

long long solveMachine(const Machine& m) {
    int n_counters = m.target_joltage.size();
    int n_buttons = m.buttons.size();
    
    vector<vector<double>> aug(n_counters, vector<double>(n_buttons + 1, 0));
    
    for (int i = 0; i < n_counters; i++) {
        for (int j = 0; j < n_buttons; j++) {
            for (int counter : m.buttons[j]) {
                if (counter == i) {
                    aug[i][j] = 1;
                    break;
                }
            }
        }
        aug[i][n_buttons] = m.target_joltage[i];
    }
    
    vector<int> pivot_col(n_counters, -1);
    int rank = 0;
    
    for (int col = 0; col < n_buttons && rank < n_counters; col++) {
        int pivot_row = -1;
        for (int row = rank; row < n_counters; row++) {
            if (abs(aug[row][col]) > 1e-9) {
                pivot_row = row;
                break;
            }
        }
        
        if (pivot_row == -1) continue;
        
        if (pivot_row != rank) {
            swap(aug[pivot_row], aug[rank]);
        }
        
        pivot_col[rank] = col;
        
        double pivot_val = aug[rank][col];
        for (int c = 0; c <= n_buttons; c++) {
            aug[rank][c] /= pivot_val;
        }
        
        for (int row = 0; row < n_counters; row++) {
            if (row != rank && abs(aug[row][col]) > 1e-9) {
                double factor = aug[row][col];
                for (int c = 0; c <= n_buttons; c++) {
                    aug[row][c] -= factor * aug[rank][c];
                }
            }
        }
        
        rank++;
    }
    
    for (int row = rank; row < n_counters; row++) {
        if (abs(aug[row][n_buttons]) > 1e-9) {
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
    
    long long min_presses = LLONG_MAX;
    int num_free = free_vars.size();
    
    long long max_target = *max_element(m.target_joltage.begin(), m.target_joltage.end());
    int search_limit = min(max_target + 10LL, 1000LL);
    
    function<void(int, vector<long long>&)> search = [&](int idx, vector<long long>& free_vals) {
        if (idx == num_free) {
            vector<long long> solution(n_buttons, 0);
            
            for (int i = 0; i < num_free; i++) {
                solution[free_vars[i]] = free_vals[i];
            }
            
            bool valid = true;
            for (int i = rank - 1; i >= 0; i--) {
                if (pivot_col[i] != -1) {
                    double val = aug[i][n_buttons];
                    for (int j = 0; j < n_buttons; j++) {
                        if (j != pivot_col[i]) {
                            val -= aug[i][j] * solution[j];
                        }
                    }
                    
                    if (abs(val - round(val)) > 0.01) {
                        valid = false;
                        break;
                    }
                    
                    long long sol_val = (long long)round(val);
                    if (sol_val < 0) {
                        valid = false;
                        break;
                    }
                    solution[pivot_col[i]] = sol_val;
                }
            }
            
            if (!valid) return;
            
            vector<long long> result(n_counters, 0);
            for (int j = 0; j < n_buttons; j++) {
                for (int counter : m.buttons[j]) {
                    if (counter < n_counters) {
                        result[counter] += solution[j];
                    }
                }
            }
            
            for (int i = 0; i < n_counters; i++) {
                if (result[i] != m.target_joltage[i]) {
                    valid = false;
                    break;
                }
            }
            
            if (valid) {
                long long total = 0;
                for (long long x : solution) {
                    total += x;
                }
                min_presses = min(min_presses, total);
            }
            return;
        }
        
        long long current_sum = 0;
        for (int i = 0; i < idx; i++) {
            current_sum += free_vals[i];
        }
        if (current_sum >= min_presses) return;
        
        for (long long val = 0; val <= search_limit; val++) {
            free_vals[idx] = val;
            search(idx + 1, free_vals);
            if (min_presses < LLONG_MAX) {
                if (val > min_presses) break;
            }
        }
    };
    
    vector<long long> free_vals(num_free, 0);
    search(0, free_vals);
    
    return (min_presses == LLONG_MAX) ? -1 : min_presses;
}

int main(int argc, char* argv[]) {
    string filename = "day9.txt";
    if (argc > 1) {
        filename = argv[1];
    }
    
    ifstream infile(filename);
    if (!infile) {
        cerr << "Could not open file '" << filename << "'\n";
        return 1;
    }
    
    long long total_presses = 0;
    string line;
    int machine_num = 0;
    int failed = 0;
    
    while (getline(infile, line)) {
        if (line.empty()) continue;
        
        machine_num++;
        Machine m = parseLine(line);
        
        long long presses = solveMachine(m);
        
        if (presses == -1) {
            cout << "Machine " << machine_num << ": No solution found" << endl;
            failed++;
        } else {
            cout << "Machine " << machine_num << ": " << presses << " presses" << endl;
            total_presses += presses;
        }
    }
    
    cout << "\nTotal button presses needed: " << total_presses << endl;
    if (failed > 0) {
        cout << "Failed machines: " << failed << endl;
    }
    
    return 0;
}
