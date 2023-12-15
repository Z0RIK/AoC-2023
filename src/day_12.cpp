#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>

// this solution uses c++ 20 features 

bool compare_vectors(const std::vector<size_t>& left, const std::vector<size_t>& right)
{
    if(left.size() != right.size()) return false;

    for(size_t i = 0; i < left.size(); i++)
        if(left[i] != right[i]) return false;
    
    return true;
}
std::unordered_map<std::string, std::vector<std::pair<std::vector<size_t>, size_t>>> dp;

size_t find_in_dp(const std::string& row, const std::vector<size_t>& groups)
{
    auto it = dp.find(row);

    if(it == dp.end()) return size_t(-1);

    auto vec_it = it->second.begin();

    while(vec_it != it->second.end() && !compare_vectors(vec_it->first, groups))
        vec_it++;
    
    if(vec_it == it->second.end()) return size_t(-1);

    return vec_it->second;
}

std::string unfold_string(const std::string& str)
{
    std::string result(str);
    for(size_t i = 0; i < 4; i++)
        result += '?' + str;
    return result;
}

std::vector<size_t> unfold_vector(const std::vector<size_t>& vec)
{
    std::vector<size_t> result(vec.size() * 5, 0);

    for(size_t i = 0; i < result.size(); i++)
        result[i] = vec[i % vec.size()];

    return result;
}


size_t row_configurations_amount(std::string_view row, std::vector<size_t> groups)
{
    if(!row.size())
        return (!groups.size()) ? 1 : 0;

    if(!groups.size())
        return (row.find('#') == row.npos) ? 1 : 0;

    size_t result = find_in_dp(std::string(row), groups);

    if(result != size_t(-1)) return result;

    result = 0;

    if(row.front() == '.' || row.front() == '?')
        result += row_configurations_amount({row.begin() + 1, row.end()}, groups);
    
    if(row.front() == '#' || row.front() == '?')
        if(groups.front() <= row.size() && row.find('.') >= groups.front() && (groups.front() == row.size() || row[groups.front()] != '#'))
            result += row_configurations_amount({row.begin() + groups.front() + ((groups.front() == row.size()) ? 0 : 1), row.end()}, {groups.begin() + 1, groups.end()});

    dp[std::string(row)].emplace_back(groups, result);
    
    return result;
}

int main(char argc, char* argv[])
{
    std::ifstream input_file;

    if(argc < 2) return 0;

    input_file.open(argv[1]);

    if(!input_file.is_open())
    {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    std::vector<std::string> input;
    std::vector<std::string> unfolded_input;
    std::vector<std::vector<size_t>> groups;
    std::vector<std::vector<size_t>> unfolded_groups;

    while(input_file.good())
    {
        input.emplace_back();
        std::getline(input_file, input.back());
        if(!input.back().size()) input.pop_back();
    }

    input_file.close();

    for(auto& str : input)
    {
        auto next_number = std::find(str.begin(), str.end(), ' ');
        std::string numbers(next_number, str.end());
        str.resize(next_number - str.begin());

        groups.emplace_back();
        next_number = numbers.begin();

        while(next_number != numbers.end())
        {
            next_number++;
            groups.back().push_back(std::stoi(std::string(next_number, numbers.end())));
            next_number = std::find(next_number, numbers.end(), ',');
        }
    }

    size_t part_1_result = 0;

    for(size_t i = 0; i < input.size(); i++)
        part_1_result += row_configurations_amount(input[i], groups[i]);

    size_t part_2_result = 0;

    for(size_t i = 0; i < input.size(); i++)
    {
        unfolded_input.emplace_back(unfold_string(input[i]));
        unfolded_groups.emplace_back(unfold_vector(groups[i]));
    }

    for(size_t i = 0; i < input.size(); i++)
        part_2_result += row_configurations_amount(unfolded_input[i], unfolded_groups[i]);


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
