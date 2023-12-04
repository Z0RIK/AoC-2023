#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <stack>

int main(char argc, char* argv[])
{
    std::ifstream input_file;
    std::vector<std::string> input;

    if (argc > 1)
        input_file.open(argv[1]);

    if (input_file.is_open())
    {
        while (input_file.good())
        {
            input.push_back(std::string());
            std::getline(input_file, input.back());
        }
    }
    else
    {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    input_file.close();

    size_t part_1_result = 0;
    size_t part_2_result = 0;

    size_t current_score = 0;
    std::bitset<100> winning_set(false);
    std::vector<size_t> amount_of_matches;
    std::stack<size_t> cards;

    for (auto str : input)
    {
        cards.push(amount_of_matches.size());
        amount_of_matches.push_back(0);

        current_score = 0;
        winning_set = false;

        auto it = std::find(str.begin(), str.end(), ':');
        while (*it != '|')
        {
            while (std::isspace(*it) || *it == ':')
                it++;

            size_t number = 0;
            while (std::isdigit(*it))
            {
                number *= 10;
                number += *it - '0';
                it++;
            }
            if (number)
                winning_set.set(number);
        }

        while (it != str.end())
        {
            while (it != str.end() && std::isspace(*it) || *it == '|')
                it++;

            size_t number = 0;
            while (it != str.end() && std::isdigit(*it))
            {
                number *= 10;
                number += *it - '0';
                it++;
            }

            if (winning_set[number])
            {
                if (!current_score)
                    current_score = 1;
                else
                    current_score <<= 1;

                amount_of_matches.back()++;
            }
        }

        part_1_result += current_score;
    }


    while (!cards.empty())
    {
        size_t current_card = cards.top();
        cards.pop();

        part_2_result++;
    
        for(size_t i = current_card; i < current_card + amount_of_matches[current_card]; i++)
            cards.push(i + 1);
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
        std::cout << "Part 1 result : " << part_1_result << std::endl;
        std::cout << "Part 2 result : " << part_2_result << std::endl;
    }

    return 0;
}
