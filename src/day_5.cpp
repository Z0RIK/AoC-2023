#include <fstream>
#include <string>
#include <vector>
#include <iostream>

struct convertion_range {
    size_t dst{}, src{}, len{};
};

class Map
{
public:

    Map() = delete;
    ~Map() = default;

    Map(std::string str)
        :name(str), ranges()
    {}

    void add_range(convertion_range range)
    {
        ranges.push_back(range);
    }

    std::vector<std::pair<size_t, size_t>> convert(std::pair<size_t, size_t> range) const
    {
        std::vector<std::pair<size_t, size_t>> result;
        std::vector<std::pair<size_t, size_t>> ranges_left;
        ranges_left.push_back(range);

        for (const auto& [map_dst, map_src, map_len] : ranges)
        {
            // check all ranges for intersections
            // intersections get converted and inserted into result vector 
            // leftovers go back to ranges_left to be checked again
            for (size_t j = 0; j < ranges_left.size(); j++)
            {
                auto [start, length] = ranges_left[j];

                if (map_src <= start && start + length <= map_src + map_len) // ||____B_||..A..||_B____|| B (map_range) fully envelops A (ranges_left[j])
                {
                    start = start - map_src + map_dst;
                    result.push_back({ start, length });
                    length -= result.back().second;
                    if (length)
                        ranges_left[j] = { start, length };
                    else {
                        ranges_left.erase(ranges_left.begin() + j);
                        j--;
                    }
                }
                else if (map_src <= start && start < map_src + map_len) // ||____A_||..I..||_B____|| A and B partially intersect
                {
                    result.push_back({ start - map_src + map_dst, map_src + map_len - start });
                    start = map_src + map_len;
                    length -= result.back().second;
                    if (length)
                        ranges_left[j] = { start, length };
                    else {
                        ranges_left.erase(ranges_left.begin() + j);
                        j--;
                    }
                }
                else if (map_src < start + length && start + length - 1 < map_src + map_len) // ||____B_||..I..||_A____|| B and A partially intersect
                {
                    result.push_back({ map_dst, start + length - 1 - map_src });
                    length -= result.back().second;
                    if (length)
                        ranges_left[j] = { start, length };
                    else {
                        ranges_left.erase(ranges_left.begin() + j);
                        j--;
                    }
                }
                else if (start < map_src && map_src + map_len < start + length) // ||____A_||..B..||_A____|| A (ranges_left[j]) fully envelops B (map_range)
                {
                    ranges_left.erase(ranges_left.begin() + j);
                    j--;

                    result.push_back({ map_dst, map_len });

                    if (map_src - start)
                        ranges_left.push_back({ start, map_src - start });

                    if (start + length - map_src - map_len)
                        ranges_left.push_back({ map_src + map_len , start + length - map_src - map_len });
                }
            }
        }
        result.insert(result.end(), ranges_left.begin(), ranges_left.end());
        return result;
    }

private:

    std::vector<convertion_range> ranges;
    std::string name;
};

inline size_t stoi(char* c)
{
    size_t result{};

    while (std::isdigit(*c))
    {
        result *= 10;
        result += *c - '0';
        c++;
    }
    return result;
}

size_t find_lowest_location(std::vector<std::pair<size_t, size_t>> seed_ranges, const std::vector<Map>& maps)
{
    size_t result = UINT64_MAX;

    while (!seed_ranges.empty())
    {
        std::vector<std::pair<size_t, size_t>> ranges_left; // ranges_left contains ranges that have not been converted
        std::vector<std::pair<size_t, size_t>> next; // next contains ranges that have been converted
        ranges_left.push_back(seed_ranges.back());
        seed_ranges.pop_back();

        for (const auto& current_map : maps)
        {
            next.resize(0);
            while (!ranges_left.empty())
            {
                std::vector<std::pair<size_t, size_t>> temp = current_map.convert(ranges_left.back());
                next.insert(next.end(), temp.begin(), temp.end());
                ranges_left.pop_back();
            }
            ranges_left = next;
        }

        for (const auto& range : ranges_left)
            result = (range.first < result) ? range.first : result;
    }

    return result;
}

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
            std::string current;
            std::getline(input_file, current);
            if (current.size())
                input.push_back(current);
        }
    }
    else
    {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    input_file.close();

    std::vector<size_t> seeds;

    for (size_t i = 0; i < input[0].size(); i++)
    {
        if (std::isdigit(input[0][i]))
        {
            seeds.push_back(0);
            while (std::isdigit(input[0][i]))
            {
                seeds.back() *= 10;
                seeds.back() += input[0][i] - '0';
                i++;
            }
        }
    }

    input.erase(input.begin());

    std::vector<Map> maps;

    for (auto& str : input)
    {
        if (std::isalpha(str[0]))
        {
            maps.emplace_back(str);
            continue;
        }

        convertion_range newmap{};
        char* c = &str[0];

        while (!std::isdigit(*c)) c++;
        newmap.dst = stoi(c);

        while (std::isdigit(*c)) c++;
        while (!std::isdigit(*c)) c++;
        newmap.src = stoi(c);

        while (std::isdigit(*c)) c++;
        while (!std::isdigit(*c)) c++;
        newmap.len = stoi(c);

        maps.back().add_range(newmap);
    }

    std::vector<std::pair<size_t, size_t>> seed_ranges;

    for (size_t i = 0; i < seeds.size(); i ++)
        seed_ranges.push_back({ seeds[i], 1 });

    size_t part_1_result = find_lowest_location(seed_ranges, maps);



    seed_ranges.resize(0);
    for (size_t i = 0; i < seeds.size(); i += 2)
        seed_ranges.push_back({ seeds[i], seeds[i + 1] });

    size_t part_2_result = find_lowest_location(seed_ranges, maps);

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
