#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <cmath>

struct Point
{
    int x, y, z;
};

struct Edge
{
    int i, j;
    double dist;
    bool operator<(const Edge &other) const
    {
        return dist < other.dist;
    }
};

class UnionFind
{
    std::vector<int> parent, size;

public:
    UnionFind(int n) : parent(n), size(n, 1)
    {
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int find(int x)
    {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int x, int y)
    {
        x = find(x);
        y = find(y);
        if (x == y)
            return false;
        if (size[x] < size[y])
            std::swap(x, y);
        parent[y] = x;
        size[x] += size[y];
        return true;
    }

    int getNumComponents()
    {
        std::set<int> roots;
        for (int i = 0; i < parent.size(); i++)
        {
            roots.insert(find(i));
        }
        return roots.size();
    }
};

double distance(const Point &a, const Point &b)
{
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    long long dz = a.z - b.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

int main()
{
    std::ifstream infile("day8.txt");
    if (!infile)
    {
        std::cerr << "Could not open file 'day8.txt'\n";
        return 1;
    }

    std::vector<Point> points;
    std::string line;
    while (std::getline(infile, line))
    {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        Point p;
        char comma;
        if (ss >> p.x >> comma >> p.y >> comma >> p.z)
        {
            points.push_back(p);
        }
    }
    infile.close();

    int n = points.size();
    std::vector<Edge> edges;

    // Calculate all pairwise distances
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            edges.push_back({i, j, distance(points[i], points[j])});
        }
    }

    // Sort edges by distance
    std::sort(edges.begin(), edges.end());

    // Keep connecting until we have only 1 component
    UnionFind uf(n);
    int lastI = -1, lastJ = -1;

    for (const auto &edge : edges)
    {
        if (uf.unite(edge.i, edge.j))
        {
            lastI = edge.i;
            lastJ = edge.j;
            if (uf.getNumComponents() == 1)
            {
                break;
            }
        }
    }

    // Multiply X coordinates of the last two junction boxes connected
    long long result = (long long)points[lastI].x * points[lastJ].x;
    std::cout << "Last connection: (" << points[lastI].x << "," << points[lastI].y << "," << points[lastI].z << ") and ("
              << points[lastJ].x << "," << points[lastJ].y << "," << points[lastJ].z << ")" << std::endl;
    std::cout << "Result: " << result << std::endl;

    return 0;
}