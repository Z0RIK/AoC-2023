#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

void tilt_north(std::vector<std::string>& input)
{
    for(size_t i = 0; i < input[0].size(); i++)
    {
        size_t edge = 0;
        for(size_t j = 0; j < input.size(); j++)
        {
            switch (input[j][i])
            {
                case 'O':
                    std::swap(input[edge][i], input[j][i]);
                    edge++;
                    break;
                case '#':
                    edge = j + 1;
                    break;
            }
        }
    }
}

void tilt_west(std::vector<std::string>& input)
{
    for(size_t i = 0; i < input.size(); i++)
    {
        size_t edge = 0;
        for(size_t j = 0; j < input[0].size(); j++)
        {
            switch (input[i][j])
            {
                case 'O':
                    std::swap(input[i][edge], input[i][j]);
                    edge++;
                    break;
                case '#':
                    edge = j + 1;
                    break;
            }
        }
    }
}

void tilt_south(std::vector<std::string>& input)
{
    for(size_t i = 0; i < input[0].size(); i++)
    {
        size_t edge = input.size() - 1;
        for(size_t j = input.size() - 1; j < size_t(-1); j--)
        {
            switch (input[j][i])
            {
                case 'O':
                    std::swap(input[edge][i], input[j][i]);
                    edge--;
                    break;
                case '#':
                    edge = j - 1;
                    break;
            }
        }
    }
}

void tilt_east(std::vector<std::string>& input)
{
    for(size_t i = 0; i < input.size(); i++)
    {
        size_t edge = input[0].size() - 1;
        for(size_t j = input[0].size() - 1; j < size_t(-1); j--)
        {
            switch (input[i][j])
            {
                case 'O':
                    std::swap(input[i][edge], input[i][j]);
                    edge--;
                    break;
                case '#':
                    edge = j - 1;
                    break;
            }
        }
    }
}

inline void run_cycle(std::vector<std::string>& input)
{
    tilt_north(input);    
    tilt_west(input);
    tilt_south(input);
    tilt_east(input);
}

size_t claculate_load(std::vector<std::string>& input)
{
    size_t result = 0;

    for(size_t i = 0; i < input.size(); i++)
        for(const auto& c : input[i])
            if(c == 'O') result += input.size() - i;
    
    return result;
}

bool compare_states(const std::vector<std::string>& left, const std::vector<std::string>& right)
{
    if(left.size() != right.size()) return false;
    for(size_t i = 0; i < left.size() && i < right.size(); i++)
        if(left[i] != right[i]) return false;
    return true;
}

int main(char argc, char* argv[])
{
    // parsing
    if(argc < 2) return 0;

    std::ifstream input_file(argv[1]);

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
    }

    input_file.close();

    // solution
    size_t part_1_result = 0;
    size_t part_2_result = 0;
    std::vector<size_t> repeating_pattern;
    std::vector<std::string> slow = input;
    std::vector<std::string> fast = input;

    // part 1
    tilt_north(slow);
    part_1_result = claculate_load(slow);

    // part 2
    size_t loop_start = 0;
    slow = input;
    fast = input;
    run_cycle(fast);

    // find loop
    while(!compare_states(fast, slow))
    {
        run_cycle(slow);
        run_cycle(fast);
        run_cycle(fast);
        
    }

    slow = input;
    loop_start = 0;

    // find loop start
    while(!compare_states(fast, slow))
    {
        run_cycle(slow);
        run_cycle(fast);
        run_cycle(fast);
        loop_start++;
    }

    repeating_pattern.push_back(claculate_load(fast));
    run_cycle(fast);

    // calculate load for states in loop 
    while(!compare_states(fast, slow))
    {
        repeating_pattern.push_back(claculate_load(fast));
        run_cycle(fast);
    }

    // get load for 1'000'000'000 nth cycle
    part_2_result = repeating_pattern[(1'000'000'000 - loop_start) % repeating_pattern.size()];
    
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
