#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

size_t compare_strings(std::string left, std::string right)
{
    size_t mistakes = 0, i;
    for(i = 0; i < left.size() && i < right.size(); i++)
        if(left[i] != right[i]) mistakes++;
    mistakes += left.size() - i;
    mistakes += right.size() - i;
    return mistakes;
}

void print_pattern(std::vector<std::string> pattern)
{
    for(const auto& str : pattern)
        std::cout << str << std::endl;
    std::cout << std::endl;
}

size_t look_horizontal(const std::vector<std::string>& pattern)
{
    for(size_t i = 1; i < pattern.size(); i++)
    {
        if(pattern[i] == pattern[i - 1])
        {
            size_t j;
            for(j = 1; (i + j) < pattern.size() && 0 < (i - j); j++)
                if(pattern[i + j] != pattern[i - j - 1]) break;
            if((i + j) == pattern.size() || (i - j) == 0)
                return i;
        }
    }

    return 0;
}

size_t look_horizontal_with_smudge(const std::vector<std::string>& pattern)
{
    for(size_t i = 1; i < pattern.size(); i++)
    {
        if(compare_strings(pattern[i], pattern[i - 1]) < 2)
        {
            size_t j;
            size_t smudge_count = compare_strings(pattern[i], pattern[i - 1]);
            for(j = 1; (i + j) < pattern.size() && 0 < (i - j) && smudge_count < 2; j++)
                smudge_count += compare_strings(pattern[i + j], pattern[i - j - 1]);
            if(((i + j) == pattern.size() || (i - j) == 0) && smudge_count == 1)
                return i;
        }
    }

    return 0;
}

std::string get_col(const size_t& index, const std::vector<std::string>& pattern)
{
    std::string result;
    result.reserve(pattern.size());

    for(const auto& str : pattern)
        result.push_back(str[index]);

    return result;
}

size_t look_vertical(const std::vector<std::string>& pattern)
{
    std::string prev;
    std::string current;
    prev.reserve(pattern.size());
    current.reserve(pattern.size());
    for(size_t i = 1; i < pattern[0].size(); i++)
    {
        prev = get_col(i - 1, pattern);
        current = get_col(i, pattern);

        if(prev == current)
        {
            size_t j;
            for(j = 1; (i + j) < pattern[0].size() && 0 < (i - j); j++)
            {
                prev = get_col(i - j - 1, pattern);
                current = get_col(i + j, pattern);
                if(current != prev) break;
            }
            if((i + j) == pattern[0].size() || (i - j) == 0)
                return i;
        }
    }

    return 0;
}

size_t look_vertical_with_smudge(const std::vector<std::string>& pattern)
{
    std::string prev;
    std::string current;
    prev.reserve(pattern.size());
    current.reserve(pattern.size());
    for(size_t i = 1; i < pattern[0].size(); i++)
    {
        prev = get_col(i - 1, pattern);
        current = get_col(i, pattern);

        if(compare_strings(prev, current) < 2)
        {
            size_t j;
            size_t smudge_count = compare_strings(prev, current);
            for(j = 1; (i + j) < pattern[0].size() && 0 < (i - j) && smudge_count < 2; j++)
            {
                prev = get_col(i - j - 1, pattern);
                current = get_col(i + j, pattern);
                smudge_count += compare_strings(prev, current);
            }
            if(((i + j) == pattern[0].size() || (i - j) == 0) && smudge_count == 1)
                return i;
        }
    }

    return 0;
}


int main(char argc, char* argv[])
{
    if(argc < 2) return 0;

    std::ifstream input_file;
    input_file.open(argv[1]);

    typedef std::vector<std::string> Pattern;

    std::vector<Pattern> patterns;

    while(input_file.good())
    {
        std::string current_string;
        std::getline(input_file, current_string);
        patterns.emplace_back();

        while(current_string.size())
        {
            patterns.back().push_back(current_string);
            if(!input_file.good()) break;
            std::getline(input_file, current_string);
        }
    }

    input_file.close();

    size_t vertical = 0;
    size_t horizontal = 0;

    for(const auto& pattern: patterns)
    {
        vertical += look_vertical(pattern);
        horizontal += look_horizontal(pattern);
    }
    size_t part_1_result = vertical + horizontal * 100;
    vertical = horizontal = 0;

    for(const auto& pattern: patterns)
    {
        vertical += look_vertical_with_smudge(pattern);
        horizontal += look_horizontal_with_smudge(pattern);
    }
    size_t part_2_result = vertical + horizontal * 100;
    std::cout << part_2_result << std::endl;

    // output
    if (argc > 2)
    {
        std::ofstream output_file;
        output_file.open(argv[2]);
        if (!output_file.is_open())
        {
            std::cerr << "Failed to open output file" << std::endl;
            return 1;
        }
        output_file << "Part 2 result : " << part_1_result << std::endl;
        output_file << "Part 2 result : " << part_2_result << std::endl;
        output_file.close();
    }
    else
    {
        std::cout << "Part 1 result : " << part_1_result << std::endl;
        std::cout << "Part 2 result : " << part_2_result << std::endl;
    }


    return 0;
}
