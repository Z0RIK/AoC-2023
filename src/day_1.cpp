#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <array>

std::array<std::string, 9> numbers = {
    "one",
    "two",
    "three", 
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

bool compare(char* c, const std::string& number)
{
    size_t i = 0;

    while (*c != '\n' && *c != '\0' && i < number.size() && *c == number[i])
    {
        c++;
        i++;
    }

    if(i == number.size()) 
        return true;
    else 
        return false;
}

int main(char argc, char* argv[])
{
    // reading from input file
    std::ifstream inputFile;
    std::stringstream inputStream;
    std::string inputString;

    if(argc > 1)
        inputFile.open(argv[1]);

    if(!inputFile.is_open()) return 0;
    inputStream << inputFile.rdbuf();
    inputString = inputStream.str();

    inputFile.close();

    // actual algorithm
    size_t first{69}, second{69}, result{};

    for(size_t i = 0; i < inputString.size(); i++)
    {
        char c = inputString[i];
        if (std::isdigit(c))
        {
            if (first == 69)
                first = c - '0';
            second = c - '0';
        }
        else if (c == '\n' || c == '\0')
        {
            result += first * 10 + second;
            first = second = 69;
        }
        else
        {
            for(size_t j = 0; j < numbers.size(); j++)
                if (compare(&inputString[i], numbers[j]))
                {
                    if (first == 69)
                        first = j + 1;
                    second = j + 1;
                }
        }
    }
    
    result += first * 10 + second; // one more add, since we exit the loop before adding last number

    // output shenanigans
    if (argc > 2)
    {
        std::ofstream outputFile;
        outputFile.open(argv[2]);

        if (outputFile.is_open())
            outputFile << result;

        outputFile.close();
    }
    else
        std::cerr << std::to_string(result) << std::endl; 

    return 0;
}
