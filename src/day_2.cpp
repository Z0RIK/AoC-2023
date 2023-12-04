#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <iostream>
#include <array>

static std::string test = "\
    Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green\n\
    Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue\n\
    Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red\n\
    Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red\n\
    Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green";

static size_t stoi(const std::string_view& str)
{
    size_t i{};
    while (i < str.size() && !std::isdigit(str[i]))
        i++;

    size_t result{};
    while (i < str.size() && std::isdigit(str[i]))
    {
        result *= 10;
        result += str[i] - '0';
        i++;
    }

    return result;
}

static inline bool is_combination_possible(std::array<size_t, 3> qubes)
{
    return qubes[0] <= 12 && qubes[1] <= 13 && qubes[2] <= 14;
}

int main(char argc, char* argv[])
{
    // reading from input file
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

    size_t part_1_result{}, part_2_result{};
    auto first = input_string.begin();
    auto last = input_string.end();

    while (first != input_string.end())
    {
        first = std::find_if(first, last, std::isdigit);
        last = std::find_if(first, last, [](int c) -> bool {return (c == '\n' || c == '\0'); });

        auto game_substr = std::string_view(first, last); // substr of currently processed game

        size_t id = stoi(game_substr);

        game_substr = std::string_view(++find(game_substr.begin(), game_substr.end(), ':'), game_substr.end()); // get game id and cut it off

        bool is_game_valid = true;
        std::array<size_t, 3> part_1_combination = { 0 };
        std::array<size_t, 3> part_2_combination = { 0 };

        for (auto it = game_substr.begin(); it != game_substr.end(); it++)
        {
            size_t number = stoi(std::string_view(it, game_substr.end())); // get number that appears first in substr
            switch (*std::find_if(it, game_substr.end(), std::isalpha)) // next character determines number qubes of wich color we are processing, no need to check for whole word
            {
            case 'r':
                part_1_combination[0] += number; 
                part_2_combination[0] = (number > part_2_combination[0]) ? number : part_2_combination[0];
                break;
            case 'g':
                part_1_combination[1] += number;
                part_2_combination[1] = (number > part_2_combination[1]) ? number : part_2_combination[1];
                break;
            case 'b':
                part_1_combination[2] += number;
                part_2_combination[2] = (number > part_2_combination[2]) ? number : part_2_combination[2];;
                break;
            }
            auto punct = std::find_if(it, game_substr.end(), std::ispunct);

            if (punct != game_substr.end() && *punct == ',')
                it = punct;
            else
            {
                is_game_valid &= is_combination_possible(part_1_combination); // check if current set satisfies rules for part 1
                part_1_combination = { 0 };
                if (punct == game_substr.end())
                    break;
                else
                    it = punct;
            }
        }
        if (is_game_valid) part_1_result += id;
        part_2_result += part_2_combination[0] * part_2_combination[1] * part_2_combination[2]; // power of set for part 2

        first = last;
        last = input_string.end();
    }

    // output shenanigans
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
