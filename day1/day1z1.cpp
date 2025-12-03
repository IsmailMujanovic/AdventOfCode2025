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
    int zerosCounter = 0;
    char direction;
    int sign = 1;
    int numberOfSteps = 0;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty())
            continue;
        std::istringstream iss(line);
        char directionChar;
        int numberOfSteps;
        if (!(iss >> directionChar >> numberOfSteps))
            continue; // parses "R12", "R 12", "R005", etc.
        int sign = (directionChar == 'R') ? 1 : (directionChar == 'L') ? -1 : 0;
        if (sign == 0)
            continue;
        currentResult += sign * numberOfSteps;
        currentResult = ((currentResult % 100) + 100) % 100;
        std::cout << line << " - " << currentResult;
        if (currentResult == 0) {
            zerosCounter++;
            std::cout << " NULA BROJ " << zerosCounter;
        }
        std::cout << std::endl;
    }

    std::cout << zerosCounter;

    return 0;
}