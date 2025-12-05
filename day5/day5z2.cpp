#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
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
    std::string line;

    while (std::getline(infile, line)) {
        if (line.empty()) break;

        size_t dashPos = line.find('-');
        if (dashPos != std::string::npos) {
            long long start = std::stoll(line.substr(0, dashPos));
            long long end = std::stoll(line.substr(dashPos + 1));
            freshRanges.push_back({start, end});
        }
    }

    std::sort(freshRanges.begin(), freshRanges.end(), [](const Range& a, const Range& b) {
        return a.start < b.start;
    });

    std::vector<Range> mergedRanges;
    for (const Range& range : freshRanges) {
        if (mergedRanges.empty() || mergedRanges.back().end < range.start - 1)
            mergedRanges.push_back(range);
        else
            mergedRanges.back().end = std::max(mergedRanges.back().end, range.end);
    }

    long long totalFreshIDs = 0;
    for (const Range& range : mergedRanges)
        totalFreshIDs += (range.end - range.start + 1);

    std::cout << "Rezultat: " << totalFreshIDs << std::endl;

    return 0;
}