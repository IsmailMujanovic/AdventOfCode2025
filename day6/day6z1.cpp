#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <map>
#include <set>
#include <algorithm>

struct NumberInfo {
    long long value;
    int startCol;
    int endCol;
    int rowIndex;
};

int main()
{
    std::ifstream infile("day6z1.txt");
    if (!infile)
    {
        std::cerr << "Could not open file 'day6z1.txt'\n";
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line))
    {
        lines.push_back(line);
    }
    infile.close();

    if (lines.size() < 2)
    {
        std::cerr << "Not enough rows in the input file\n";
        return 1;
    }

    std::string operatorLine = lines.back();
    std::vector<std::string> numberLines(lines.begin(), lines.end() - 1);

    std::vector<NumberInfo> allNumbers;
    
    for (size_t rowIdx = 0; rowIdx < numberLines.size(); rowIdx++)
    {
        const auto& numLine = numberLines[rowIdx];
        std::string currentNumber = "";
        int startCol = -1;
        
        for (size_t col = 0; col < numLine.length(); col++)
        {
            char c = numLine[col];
            if (std::isdigit(c))
            {
                if (currentNumber.empty())
                    startCol = col;
                currentNumber += c;
            }
            else
            {
                if (!currentNumber.empty())
                {
                    long long value = std::stoll(currentNumber);
                    int endCol = col - 1;
                    allNumbers.push_back({value, startCol, endCol, (int)rowIdx});
                    currentNumber = "";
                }
            }
        }
        if (!currentNumber.empty())
        {
            long long value = std::stoll(currentNumber);
            int endCol = numLine.length() - 1;
            allNumbers.push_back({value, startCol, endCol, (int)rowIdx});
        }
    }

    int maxCols = operatorLine.length();
    std::vector<bool> isSpaceColumn(maxCols, true);
    
    for (size_t col = 0; col < maxCols; col++)
    {
        for (const auto& numLine : numberLines)
        {
            if (col < numLine.length() && numLine[col] != ' ')
            {
                isSpaceColumn[col] = false;
                break;
            }
        }
    }
    
    std::vector<std::pair<int, int>> problemRanges;
    int start = -1;
    for (size_t col = 0; col < maxCols; col++)
    {
        if (!isSpaceColumn[col] && start == -1)
        {
            start = col;
        }
        else if (isSpaceColumn[col] && start != -1)
        {
            problemRanges.push_back({start, (int)col - 1});
            start = -1;
        }
    }
    if (start != -1)
    {
        problemRanges.push_back({start, maxCols - 1});
    }
    
    long long grandTotal = 0;
    
    for (const auto& [startCol, endCol] : problemRanges)
    {
        std::vector<NumberInfo> problemNumbers;
        for (const auto& numInfo : allNumbers)
        {
            if (!(numInfo.endCol < startCol || numInfo.startCol > endCol))
            {
                problemNumbers.push_back(numInfo);
            }
        }
        
        char op = ' ';
        for (int col = startCol; col <= endCol; col++)
        {
            if (col < operatorLine.length())
            {
                char c = operatorLine[col];
                if (c == '+' || c == '*')
                {
                    op = c;
                    break;
                }
            }
        }
        
        if (!problemNumbers.empty() && op != ' ')
        {
            std::sort(problemNumbers.begin(), problemNumbers.end(),
                     [](const NumberInfo& a, const NumberInfo& b) {
                         return a.rowIndex < b.rowIndex;
                     });
            
            long long result = problemNumbers[0].value;
            for (size_t i = 1; i < problemNumbers.size(); i++)
            {
                if (op == '+')
                    result += problemNumbers[i].value;
                else if (op == '*')
                    result *= problemNumbers[i].value;
            }
            grandTotal += result;
        }
    }

    std::cout << grandTotal << std::endl;

    return 0;
}