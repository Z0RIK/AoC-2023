#include <fstream>
#include <string>
#include <vector>
#include <iostream>

size_t count_possible_wins(size_t time, size_t distance)
{
    size_t t1 = 0;
    size_t t2 = time;

    while (t1 * (time - t1) <= distance)
        t1++;

    while (t2 * (time - t2) <= distance)
        t2--;

    return t2 - t1 + 1;
}

int main(char argc, char* argv[])
{
    std::ifstream input_file;
    std::vector<std::string> input;

    if (argc > 1)
        input_file.open(argv[1]);
    else
        return 1;

    while (input_file.good())
    {
        input.push_back("");
        std::getline(input_file, input.back(), ' ');
        if (input.back() == "") input.pop_back();
    }

    std::vector<size_t> times;
    std::vector<size_t> distances;
    std::string part_2_time;
    std::string part_2_distance;
    size_t part_1_result{};
    size_t part_2_result{};

    auto str = ++input.begin();

    while (str != input.end())
    {
        times.push_back(std::stoi(*str));
        if (!isdigit((*str).back())) break;
        str++;
    }
    str++;
    while (str != input.end())
    {
        distances.push_back(std::stoi(*str));
        str++;
    }

    for (const auto& number : times)
        part_2_time += (std::to_string(number));

    for (const auto& number : distances)
        part_2_distance += (std::to_string(number));

    for (size_t i = 0; i < times.size(); i++)
    {
        size_t temp = count_possible_wins(times[i], distances[i]);
        if(temp)
        {
            if(!part_1_result)
                part_1_result = temp;
            else
                part_1_result *= temp;
        }
    }

    part_2_result = count_possible_wins(std::stoll(part_2_time), std::stoll(part_2_distance));

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
