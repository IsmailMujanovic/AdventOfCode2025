#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main()
{
    std::ifstream infile("day3z1Banks.txt");
    if (!infile)
    {
        std::cerr << "Could not open file 'day3z1Banks.txt'\n";
        return 1;
    }

    int currentResult = 0;
    std::string line;
    while (std::getline(infile, line))
    {
        if (line.empty())
            continue;

        int max = 0;
        for (int i = 0; i < line.size() - 1; i++) {
            unsigned char chi = static_cast<unsigned char>(line[i]);
            if (std::isdigit(chi)) {
                int numberi = chi - '0';
                for (int j = i + 1; j < line.size(); j++) {
                    unsigned char chj = static_cast<unsigned char>(line[j]);
                    if (std::isdigit(chj)) {
                        int numberj = chj - '0';
                        int val = numberi * 10 + numberj;
                        if (val > max) max = val;
                    }
                }
            }
        }

        currentResult += max;
    }

    std::cout << std::endl << "Rezultat: " << currentResult;

    return 0;
}