#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

struct Point
{
    int x, y;
};

int main()
{
    std::ifstream infile("day9.txt");
    if (!infile)
    {
        std::cerr << "Could not open file 'day9.txt'\n";
        return 1;
    }

    std::vector<Point> redTiles;
    std::string line;
    while (std::getline(infile, line))
    {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        Point p;
        char comma;
        if (ss >> p.x >> comma >> p.y)
        {
            redTiles.push_back(p);
        }
    }
    infile.close();

    int n = redTiles.size();
    std::cout << "Number of red tiles: " << n << std::endl;

    long long maxArea = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            long long width = std::abs(redTiles[i].x - redTiles[j].x) + 1;
            long long height = std::abs(redTiles[i].y - redTiles[j].y) + 1;
            long long area = width * height;
            
            if (area > maxArea)
            {
                maxArea = area;
            }
        }
    }

    std::cout << "Largest rectangle area: " << maxArea << std::endl;

    return 0;
}