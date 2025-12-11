#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>

std::map<std::tuple<std::string, bool, bool>, long long> memo;

long long countPathsDP(const std::string& current, 
                       const std::string& end,
                       const std::map<std::string, std::vector<std::string>>& graph,
                       bool hasDac,
                       bool hasFft)
{
    if (current == "dac") hasDac = true;
    if (current == "fft") hasFft = true;
    
    if (current == end)
    {
        return (hasDac && hasFft) ? 1 : 0;
    }
    
    auto key = std::make_tuple(current, hasDac, hasFft);
    auto it = memo.find(key);
    if (it != memo.end())
    {
        return it->second;
    }
    
    long long totalPaths = 0;
    
    auto neighbors = graph.find(current);
    if (neighbors != graph.end())
    {
        for (const std::string& neighbor : neighbors->second)
        {
            totalPaths += countPathsDP(neighbor, end, graph, hasDac, hasFft);
        }
    }
    
    memo[key] = totalPaths;
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

    long long pathCount = countPathsDP("svr", "out", graph, false, false);
    
    std::cout << "Number of paths from 'svr' to 'out' visiting both 'dac' and 'fft': " << pathCount << std::endl;

    return 0;
}
