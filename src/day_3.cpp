#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

static std::string test = "\
467..114..\n\
...*......\n\
..35..633.\n\
......#...\n\
617*......\n\
.....+.58.\n\
..592.....\n\
......755.\n\
...$.*....\n\
.664.598..";

static std::vector<std::pair<int, int>> indexes_to_check = {
    {-1, -1}, {-1, 0}, {-1, 1},
    { 0, -1},          { 0, 1},
    { 1, -1}, { 1, 0}, { 1, 1}
};

bool is_part_number(size_t y, size_t x, const std::vector<std::string>& matrix);
inline bool is_symbol(char c);

int main(char argc, char* argv[])
{
    std::ifstream input_file;
    std::stringstream input_stream;
    std::string input_string;

    if (argc > 1)
        input_file.open(argv[1]);

    if (!input_file.is_open())
    {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    input_stream << input_file.rdbuf();
    input_string = input_stream.str();

    input_file.close();

    //input_string = test;
    std::vector<std::string> matrix;
    size_t part_1_result = 0;
    size_t part_2_result = 0;

    for (auto it = input_string.begin(); it != input_string.end(); )
    {
        auto new_line = std::find(it, input_string.end(), '\n');
        matrix.emplace_back(it, new_line);
        it = new_line;
        if(new_line != input_string.end()) it++;
    }

    for (size_t y = 0; y < matrix.size(); y++)
    {
        for (size_t x = 0; x < matrix[y].size();)
        {
            while(x < matrix[y].size() && !std::isdigit(matrix[y][x]))
                x++;

            if(x == matrix[y].size())
                break;

            size_t part_number = 0;
            size_t number_end = x;
            while (number_end < matrix[y].size() && std::isdigit(matrix[y][number_end]))
            {
                part_number *= 10;
                part_number += static_cast<size_t>(matrix[y][number_end] - '0');
                number_end++;
            }

            if (is_part_number(y, x, matrix))
                part_1_result += part_number;

            x = number_end;
        }
    }

    std::vector<size_t> part_numbers;
    for (size_t y = 0; y < matrix.size(); y++)
    {
        for (size_t x = 0; x < matrix[y].size(); x++)
        {
            if(matrix[y][x] != '*') continue;

            part_numbers.resize(0);

            for (const auto& [offset_y, offset_x] : indexes_to_check)
            {
                int next_y = y + offset_y;
                int next_x = x + offset_x;

                if ((0 <= next_y && next_y < matrix.size()) && (0 <= next_x && next_x < matrix[0].size()) && std::isdigit(matrix[next_y][next_x]))
                {
                    while (next_x > 0)
                    {
                        if(!std::isdigit(matrix[next_y][next_x - 1])) break;
                        next_x--;
                    }

                    if(std::find(part_numbers.begin(), part_numbers.end(), (next_y * matrix[y].size() + next_x)) == part_numbers.end())
                        part_numbers.push_back(next_y * matrix[y].size() + next_x);
                }
            }

            if (part_numbers.size() == 2)
            {
                size_t power = 1;

                for (auto part_number_index : part_numbers)
                {
                    size_t part_number = 0;
                    size_t temp_y = part_number_index / matrix[y].size();
                    size_t temp_x = part_number_index % matrix[y].size();

                    while (temp_x < matrix[temp_y].size() && std::isdigit(matrix[temp_y][temp_x]))
                    {
                        part_number *= 10;
                        part_number += static_cast<size_t>(matrix[temp_y][temp_x] - '0');
                        temp_x++;
                    }

                    power *= part_number;
                }

                part_2_result += power;
            }
        }
    }

    if (argc > 2)
    {
        std::ofstream output_file;
        output_file.open(argv[2]);

        if (!output_file.is_open())
        {
            std::cerr << "Failed to open output file" << std::endl;
            return 1;
        }
        
        output_file << "Part 1 result : " << part_1_result << std::endl;
        output_file << "Part 2 result : " << part_2_result << std::endl;

        output_file.close();
    }
    else
    {
        std::cerr << "Part 1 result : " << part_1_result << std::endl;
        std::cerr << "Part 2 result : " << part_2_result << std::endl;
    }

    return 0;
}

bool is_part_number(size_t y, size_t x, const std::vector<std::string>& matrix)
{
    for (const auto& [offset_y, offset_x] : indexes_to_check)
    {
        int next_y = y + offset_y;
        int next_x = x + offset_x;

        if((0 <= next_y && next_y < matrix.size()) && (0 <= next_x && next_x < matrix[0].size()) && (is_symbol(matrix[next_y][next_x])))
            return true;
    }

    if(x + 1 < matrix[y].size() && std::isdigit(matrix[y][x + 1]))
        return is_part_number(y, x + 1, matrix);

    return false;
}

inline bool is_symbol(char c)
{
    return !std::isdigit(c) && c != '.';
}
