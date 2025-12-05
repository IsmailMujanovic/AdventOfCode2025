#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>

struct Range {
    long long start;
    long long end;
};

int main() {
    std::ifstream infile("day5z1.txt");
    if (!infile) {
        std::cerr << "Could not open file 'day5z1.txt'\n";
        return 1;
    }

    std::vector<Range> freshRanges;
    std::vector<long long> availableIDs;
    std::string line;
    bool readingRanges = true;

    while (std::getline(infile, line)) {
        if (line.empty()) {
            readingRanges = false;
            continue;
        }

        if (readingRanges) {
            size_t dashPos = line.find('-');
            if (dashPos != std::string::npos) {
                long long start = std::stoll(line.substr(0, dashPos));
                long long end = std::stoll(line.substr(dashPos + 1));
                freshRanges.push_back({start, end});
            }
        }
        else {
            availableIDs.push_back(std::stoll(line));
        }
    }

    int freshCount = 0;
    for (long long id : availableIDs) {
        bool isFresh = false;
        for (const Range& range : freshRanges) {
            if (id >= range.start && id <= range.end) {
                isFresh = true;
                break;
            }
        }
        if (isFresh) freshCount++;
    }

    std::cout << "Rezultat: " << freshCount << std::endl;

    return 0;
}