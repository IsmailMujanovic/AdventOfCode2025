#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

struct Point
{
    long long x, y;
};

bool isInsideOrOnPolygon(const Point &p, const std::vector<Point> &polygon)
{
    int n = polygon.size();
    
    for (int i = 0; i < n; i++)
    {
        if (polygon[i].x == p.x && polygon[i].y == p.y)
            return true;
    }
    
    for (int i = 0; i < n; i++)
    {
        Point p1 = polygon[i];
        Point p2 = polygon[(i + 1) % n];
        
        if (p1.y == p2.y && p.y == p1.y)
        {
            long long minX = std::min(p1.x, p2.x);
            long long maxX = std::max(p1.x, p2.x);
            if (p.x >= minX && p.x <= maxX)
                return true;
        }
        if (p1.x == p2.x && p.x == p1.x)
        {
            long long minY = std::min(p1.y, p2.y);
            long long maxY = std::max(p1.y, p2.y);
            if (p.y >= minY && p.y <= maxY)
                return true;
        }
    }
    

    int crossings = 0;
    for (int i = 0; i < n; i++)
    {
        Point p1 = polygon[i];
        Point p2 = polygon[(i + 1) % n];
        
        if (p1.x == p2.x)
        {
            long long minY = std::min(p1.y, p2.y);
            long long maxY = std::max(p1.y, p2.y);
            
            if (p1.x > p.x && p.y >= minY && p.y < maxY)
            {
                crossings++;
            }
        }
    }
    
    return (crossings % 2) == 1;
}

bool isRectangleValid(long long x1, long long y1, long long x2, long long y2, 
                      const std::vector<Point> &polygon, const std::vector<Point> &redTiles)
{
    long long minX = std::min(x1, x2);
    long long maxX = std::max(x1, x2);
    long long minY = std::min(y1, y2);
    long long maxY = std::max(y1, y2);
    
    Point corners[4] = {
        {minX, minY},
        {minX, maxY},
        {maxX, minY},
        {maxX, maxY}
    };
    
    for (int i = 0; i < 4; i++)
    {
        if (!isInsideOrOnPolygon(corners[i], polygon))
            return false;
    }

    int n = polygon.size();
    for (int i = 0; i < n; i++)
    {
        Point p1 = polygon[i];
        Point p2 = polygon[(i + 1) % n];
        
        if (p1.y == p2.y)
        {
            long long edgeMinX = std::min(p1.x, p2.x);
            long long edgeMaxX = std::max(p1.x, p2.x);
            
            if (p1.y > minY && p1.y < maxY)
            {
                if (edgeMinX < maxX && edgeMaxX > minX)
                {
                    return false;
                }
            }
        }
        else if (p1.x == p2.x)
        {
            long long edgeMinY = std::min(p1.y, p2.y);
            long long edgeMaxY = std::max(p1.y, p2.y);
            
            if (p1.x > minX && p1.x < maxX)
            {
                if (edgeMinY < maxY && edgeMaxY > minY)
                {
                    return false;
                }
            }
        }
    }
    
    return true;
}

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
    int validCount = 0;
    int totalPairs = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            totalPairs++;
            if (isRectangleValid(redTiles[i].x, redTiles[i].y, 
                               redTiles[j].x, redTiles[j].y, redTiles, redTiles))
            {
                validCount++;
                long long width = std::abs(redTiles[i].x - redTiles[j].x) + 1;
                long long height = std::abs(redTiles[i].y - redTiles[j].y) + 1;
                long long area = width * height;
                
                if (area > maxArea)
                {
                    maxArea = area;
                }
            }
        }
    }

    std::cout << "Total pairs: " << totalPairs << std::endl;
    std::cout << "Valid rectangles: " << validCount << std::endl;
    std::cout << "Largest rectangle area: " << maxArea << std::endl;

    return 0;
}