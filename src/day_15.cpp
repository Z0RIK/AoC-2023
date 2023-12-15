#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>

uint8_t get_hash(const std::string& str)
{
    uint8_t hash = 0;
    for(const auto& c : str)
    {
        hash += c;
        hash *= 17;
    }
    return hash;
}

class Box
{
public:

    void add(const std::string& label, const uint8_t& focal_length)
    {
        auto it = find(label);
        if(it == m_lenses.end())
            m_lenses.push_back({label, focal_length});
        else
            it->second = focal_length;
    }

    void remove(const std::string& label)
    {
        auto it = find(label);
        if(it != m_lenses.end())
            m_lenses.erase(it);
    }

    size_t get_sum()
    {
        size_t sum = 0;
        for(size_t i = 0; i < m_lenses.size(); i++)
            sum += m_lenses[i].second * (1 + i);
        return sum;
    }

private:

    std::vector<std::pair<std::string, uint8_t>> m_lenses;

private:

    std::vector<std::pair<std::string, uint8_t>>::iterator find(const std::string& label)
    {
        std::vector<std::pair<std::string, uint8_t>>::iterator it;
        for(it = m_lenses.begin(); it != m_lenses.end(); it++)
            if(it->first == label) break;
        return it;
    }
};

int main(char argc, char* argv[])
{
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
        std::getline(input_file, input.back(), ',');
        if(input.back().size() == 0) input.pop_back();
    }

    input_file.close();

    size_t part_1_result = 0;
    size_t part_2_result = 0;
    std::array<Box, 256> boxes = { Box() };
    
    // part 1
    for(const auto& str : input)
        part_1_result += get_hash(str);

    // part 2
    for(const auto& str : input)
    {
        auto operation_it = std::find_if(str.begin(), str.end(), [](char c)->bool{ return c == '=' || c == '-';});
        std::string label(str.begin(), operation_it);
        uint8_t hash = get_hash(label);
        char operation = (*operation_it);
        uint8_t focal_length = ((++operation_it) != str.end()) ? (*operation_it) - '0' : 0;

        switch (operation) 
        {
            case '-':
                boxes[hash].remove(label);
                break;
            case '=':
                boxes[hash].add(label, focal_length);
                break;
        }
    }

    for(size_t i = 0; i < boxes.size(); i++)
        part_2_result += boxes[i].get_sum() * (i + 1);

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
