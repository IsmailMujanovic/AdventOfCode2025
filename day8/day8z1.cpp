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

    std::vector<int> getCircuitSizes()
    {
        std::map<int, int> circuits;
        for (int i = 0; i < parent.size(); i++)
        {
            circuits[find(i)]++;
        }
        std::vector<int> sizes;
        for (const auto &p : circuits)
        {
            sizes.push_back(p.second);
        }
        return sizes;
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
    std::cout << "Number of junction boxes: " << n << std::endl;

    std::vector<Edge> edges;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            edges.push_back({i, j, distance(points[i], points[j])});
        }
    }

    std::cout << "Number of edges: " << edges.size() << std::endl;

    std::sort(edges.begin(), edges.end());

    UnionFind uf(n);
    int connectionsNeeded = 1000;
    int edgesProcessed = 0;
    int successfulConnections = 0;

    for (const auto &edge : edges)
    {
        if (edgesProcessed >= connectionsNeeded)
            break;
        edgesProcessed++;
        if (uf.unite(edge.i, edge.j))
        {
            successfulConnections++;
        }
    }

    std::cout << "Edges processed: " << edgesProcessed << std::endl;
    std::cout << "Successful connections: " << successfulConnections << std::endl;

    std::vector<int> sizes = uf.getCircuitSizes();
    std::sort(sizes.rbegin(), sizes.rend());

    std::cout << "Number of circuits: " << sizes.size() << std::endl;
    std::cout << "Top 3 circuit sizes: ";
    for (int i = 0; i < std::min(3, (int)sizes.size()); i++)
    {
        std::cout << sizes[i] << " ";
    }
    std::cout << std::endl;

    if (sizes.size() >= 3)
    {
        long long result = (long long)sizes[0] * sizes[1] * sizes[2];
        std::cout << "Result: " << result << std::endl;
    }
    else
    {
        std::cout << "Not enough circuits!" << std::endl;
    }

    return 0;
}