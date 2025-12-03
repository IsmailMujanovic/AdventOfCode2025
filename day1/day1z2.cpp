#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main()
{
    std::ifstream infile("day1z1Password.txt");
    if (!infile) {
        std::cerr << "Could not open file 'day1z1Password.txt'\n";
        return 1;
    }

    int currentResult = 50;
    long long zerosCounter = 0;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        char directionChar;
        long long numberOfSteps;
        if (!(iss >> directionChar >> numberOfSteps)) continue; // parses "R12", "R 12", "R005", etc.
        int sign = (directionChar == 'R') ? 1 : (directionChar == 'L') ? -1 : 0;
        if (sign == 0) continue;

        long long crossings = 0;
        if (sign == 1) {
            crossings = (currentResult + numberOfSteps) / 100;
        } else {
            if (numberOfSteps >= currentResult)
                crossings = (currentResult != 0 ? 1 : 0) + (numberOfSteps - currentResult) / 100;
        }
        zerosCounter += crossings;

        long long tmp = static_cast<long long>(currentResult) + sign * numberOfSteps;
        currentResult = static_cast<int>(((tmp % 100) + 100) % 100);
        std::cout << line << " - " << currentResult;
        
        std::cout << std::endl;
    }

    std::cout << std::endl << "Rezultat: " << zerosCounter;

    return 0;
}