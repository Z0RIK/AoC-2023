#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

bool check_for_zeroes(const std::vector<int>& dif_sequence)
{
    for(const auto& number : dif_sequence)
        if(number) return false;
    return true;
}

std::vector<std::vector<int>> calculate_diferences(const std::vector<int>& report)
{
    std::vector<std::vector<int>> result;
    result.push_back(report);

    while(!check_for_zeroes(result.back()))
    {
        std::vector<int> new_dif;

        for(size_t i = 0; i < result.back().size() - 1; i++)
            new_dif.push_back(result.back()[i + 1] - result.back()[i]);

        result.push_back(new_dif);
    }

    return result;
}

std::pair<int, int> extrapolate_report(const std::vector<int>& report)
{
    std::vector<std::vector<int>> dif_sequences = calculate_diferences(report);
    // part 1 
    for(auto it = dif_sequences.rbegin(); it != dif_sequences.rend(); it++)
    {
        if(it == dif_sequences.rbegin())
        {
            it->push_back(0);
            continue;
        }

        it->push_back(it->back() + (it - 1)->back());
    }
    // part 2 
    for(auto it = dif_sequences.rbegin(); it != dif_sequences.rend(); it++)
    {
        if(it == dif_sequences.rbegin())
        {
            it->insert(it->begin(), 0);
            continue;
        }

        it->insert(it->begin() ,it->front() - (it - 1)->front());
    }

    return {dif_sequences.front().front(), dif_sequences.front().back()};
}

int main(char argc, char* argv[])
{
    std::ifstream input_file;

    if(argc > 1)
        input_file.open(argv[1]);
    
    if(!input_file.is_open())
    {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> reports;

    while(input_file.good())
    {
        std::string current_history;
        std::getline(input_file, current_history);

        if(!current_history.size()) continue;

        reports.emplace_back();

        for(auto it = current_history.begin(); it != current_history.end();)
        {
            reports.back().push_back(std::stoi(std::string(it, current_history.end())));
            it = std::find(++it, current_history.end(), ' ');
        }
    }

    input_file.close();

    int part_1_result = 0;
    int part_2_result = 0;
    for(const auto& report : reports)
    {
        const auto [front, back] = extrapolate_report(report);
        part_1_result += back;
        part_2_result += front;
    }

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
