#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>

int main()
{
    std::ifstream infile("day3z1Banks.txt");
    if (!infile)
    {
        std::cerr << "Could not open file 'day3z1Banks.txt'\n";
        return 1;
    }

    long long currentResult = 0;
    std::string line;
    const int K = 12;
    while (std::getline(infile, line))
    {
        if (line.empty()) continue;

        std::string resultString;
        resultString.reserve(K);
        int minimumIndex = 0;
        int n = static_cast<int>(line.size());
        for (int i = 0; i < K; i++) {
            int maxDigit = -1;
            int bestPos = -1;
            int maxJ = n - (K - i);
            for (int j = minimumIndex; j <= maxJ; j++) {
                unsigned char ch = static_cast<unsigned char>(line[j]);
                if (!std::isdigit(ch)) continue;
                int number = ch - '0';
                if (number > maxDigit) {
                    maxDigit = number;
                    bestPos = j;
                    if (maxDigit == 9) break;
                }
            }
            if (bestPos == -1) { resultString.clear(); break; }
            resultString.push_back(line[bestPos]);
            minimumIndex = bestPos + 1;
        }

        if (!resultString.empty())
            currentResult += std::stoll(resultString);
    }

    std::cout << std::endl
              << "Rezultat: " << currentResult;

    return 0;
}