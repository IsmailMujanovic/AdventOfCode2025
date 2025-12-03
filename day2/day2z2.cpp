#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

static inline std::string trim(const std::string &s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool repeatingDigits(long long n) {
    if (n < 0) n = -n;
    std::string s = std::to_string(n);
    size_t len = s.size();
    if (len <= 1) return false;

    for (size_t L = 1; L <= len / 2; ++L) {
        if (len % L != 0) continue;
        size_t repeats = len / L;
        bool ok = true;
        for (size_t r = 1; r < repeats; ++r) {
            if (s.compare(0, L, s, r * L, L) != 0) { ok = false; break; }
        }
        if (ok) return true;
    }

    return false;
}


int main()
{
    std::ifstream infile("day2z1IDs.txt");
    if (!infile)
    {
        std::cerr << "Could not open file 'day2z1IDs.txt'\n";
        return 1;
    }

    long long result = 0;
    std::string pair;
    // split by comma to get "number1-number2" chunks
    while (std::getline(infile, pair, ','))
    {
        pair = trim(pair);
        if (pair.empty())
            continue;

        auto dashPos = pair.find('-');
        if (dashPos == std::string::npos)
            continue;

        std::string s1 = trim(pair.substr(0, dashPos));
        std::string s2 = trim(pair.substr(dashPos + 1));

        try
        {
            long long number1 = std::stoll(s1);
            long long number2 = std::stoll(s2);
            // handle the two numbers here
            std::cout << "got: " << number1 << " and " << number2 << '\n';
            for (long long i = number1; i <= number2; i++) {
                if (repeatingDigits(i)) result += i;
            }
        }
        catch (const std::exception &)
        {
            // skip malformed pairs
            continue;
        }
    }

    std::cout << "Result: " << result << std::endl;

    return 0;
}