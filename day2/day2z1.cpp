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

int numberOfDigits(long long n) {
    if (n < 0) n = -n;
    if (n == 0) return 1; // 0 has 1 digit
    int count = 0;
    while (n != 0) { n /= 10; ++count; }
    return count;
}

std::string firstHalf_str(long long n) {
    if (n < 0) n = -n;
    std::string s = std::to_string(n);
    if (s == "0") return "0";
    size_t half = s.size() / 2;
    return s.substr(0, half);
}

std::string secondHalf_str(long long n) {
    if (n < 0) n = -n;
    std::string s = std::to_string(n);
    if (s == "0") return "0";
    size_t half = s.size() / 2;
    return s.substr(half);
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
                int counterOfDigits = numberOfDigits(i);
                if (counterOfDigits % 2 == 0) {
                    if (firstHalf_str(i) == secondHalf_str(i)) {
                        result += i;
                    }
                }
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