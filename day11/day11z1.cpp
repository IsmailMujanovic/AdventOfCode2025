#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>

long long countPaths(const std::string& current, 
                     const std::string& end,
                     const std::map<std::string, std::vector<std::string>>& graph,
                     std::set<std::string>& visited)
{
    if (current == end)
    {
        return 1;
    }
    
    visited.insert(current);
    
    long long totalPaths = 0;
    
    auto it = graph.find(current);
    if (it != graph.end())
    {
        for (const std::string& neighbor : it->second)
        {
            if (visited.find(neighbor) == visited.end())
            {
                totalPaths += countPaths(neighbor, end, graph, visited);
            }
        }
    }
    
    visited.erase(current);
    
    return totalPaths;
}

int main()
{
    std::ifstream infile("day11.txt");
    if (!infile)
    {
        std::cerr << "Could not open file 'day11.txt'\n";
        return 1;
    }

    std::map<std::string, std::vector<std::string>> graph;
    
    std::string line;
    while (std::getline(infile, line))
    {
        if (line.empty())
            continue;
            
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos)
            continue;
            
        std::string device = line.substr(0, colonPos);
        std::string outputs = line.substr(colonPos + 1);
        
        std::stringstream ss(outputs);
        std::string output;
        while (ss >> output)
        {
            graph[device].push_back(output);
        }
    }
    infile.close();

    std::set<std::string> visited;
    long long pathCount = countPaths("you", "out", graph, visited);
    
    std::cout << "Number of different paths from 'you' to 'out': " << pathCount << std::endl;

    return 0;
}
