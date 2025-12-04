#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>

int main()
{
    std::ifstream infile("day4z1RollsOfPaper.txt");
    if (!infile)
    {
        std::cerr << "Could not open file 'day4z1RollsOfPaper.txt'\n";
        return 1;
    }

    std::vector<std::string> grid;
    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
            grid.push_back(line);
    }

    int accessibleCount = 0;
    int rows = grid.size();

    for (int i = 0; i < rows; i++)
    {
        int cols = grid[i].size();
        for (int j = 0; j < cols; j++)
        {
            if (grid[i][j] != '@') continue;

            int adjacentRolls = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    if (di == 0 && dj == 0) continue;

                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < rows && nj >= 0 && nj < (int)grid[ni].size()) {
                        if (grid[ni][nj] == '@') adjacentRolls++;
                    }
                }
            }
            if (adjacentRolls < 4) accessibleCount++;
        }
    }

    std::cout << std::endl
              << "Rezultat: " << accessibleCount;

    return 0;
}