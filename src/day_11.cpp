#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_set>

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

    std::vector<std::string> input;

    while(input_file.good())
    {
        input.emplace_back();
        std::getline(input_file, input.back());
        if(!input.back().size()) input.pop_back();
    }

    input_file.close();

    std::vector<size_t> galaxies;
    std::unordered_set<size_t> rows_to_expand;
    std::unordered_set<size_t> cols_to_expand;

    size_t part_1_result = 0;
    size_t part_2_result = 0;

    for(size_t i = 0; i < input.size() || i < input[0].size(); i++)
    {
        if(i < input.size())
            rows_to_expand.insert(i);
        if(i < input[0].size())
            cols_to_expand.insert(i);
    }

    for(size_t i = 0; i < input.size(); i++)
        for(size_t j = 0; j < input[0].size(); j++)
            if(input[i][j] == '#')
            {
                if(rows_to_expand.count(i)) rows_to_expand.erase(i);
                if(cols_to_expand.count(j)) cols_to_expand.erase(j);
                galaxies.push_back(i * input[0].size() + j);
            }

    for(size_t i = galaxies.size() - 1; 0 < i; i--)
    {
        size_t from_x, from_y;
        from_y = galaxies[i] / input[0].size();
        from_x = galaxies[i] % input[0].size();
        galaxies.pop_back();
        for(size_t j = 0; j < galaxies.size(); j++)
        {
            size_t to_x, to_y, length{}, expansion{};
            to_y = galaxies[j] / input[0].size();
            to_x = galaxies[j] % input[0].size();

            while(to_x != from_x)
            {
                if(to_x < from_x)
                    to_x++;
                else
                    to_x--;
                if(cols_to_expand.count(to_x)) expansion++;
                length++;
            }

            while(to_y != from_y)
            {
                if(to_y < from_y)
                    to_y++;
                else
                    to_y--;
                if(rows_to_expand.count(to_y)) expansion++;
                length++;
            }
            part_1_result += length + expansion;
            part_2_result += length + expansion * 999'999;
        }
        
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
