#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <optional>

std::optional<std::vector<std::string>> get_input(std::string path)
{
    std::ifstream input_file(path);

    if(!input_file.is_open())
    {
        std::cerr << "Failed to open input file" << std::endl;
        return std::nullopt;
    }

    std::vector<std::string> result;

    while(input_file.good())
    {
        result.emplace_back();
        std::getline(input_file, result.back());
    }

    return result;
}

enum Direction{
    none = 0, 
    down = (0x1 << 2),
    right = (0x1 << 3),
    up = (0x1 << 2) + 0x1,
    left = (0x1 << 3) + 0x1
};

inline std::pair<Direction, Direction> change_dir(const char& c, Direction dir)
{
    std::pair<Direction, Direction> result{Direction::none, Direction::none};

    switch (c)
    {
    case '|':
        if(dir == left || dir == right)
            return {up, down};
        break;
    case '-':
        if(dir == up || dir == down)
            return {left, right};
        break;
    case '\\':
        if(dir == left || dir == right)
            return {Direction(uint8_t(dir) ^ (0x1 << 3) ^ (0x1 << 2)), none};
        if(dir == up || dir == down)
            return {Direction(uint8_t(dir) ^ (0x1 << 3) ^ (0x1 << 2)), none};
        break;
    case '/':
        if(dir == left || dir == right)
            return {Direction(uint8_t(dir) ^ 0x1 ^ (0x1 << 3) ^ (0x1 << 2)), none};
        if(dir == up || dir == down)
            return {Direction(uint8_t(dir) ^ 0x1 ^ (0x1 << 3) ^ (0x1 << 2)), none};
        break;
    }

    return {dir, none};
}

inline std::pair<size_t, size_t> direction_to_coords(const size_t& y, const size_t& x, const Direction& dir)
{
    switch(dir)
    {
        case down:
            return {y + 1, x};
            break;
        case right:
            return {y, x + 1};
            break;
        case up:
            return {y - 1, x};
            break;
        case left:
            return {y, x - 1}; 
            break;
    }

    return {y, x};
}

void calculate_light_path(const std::vector<std::string>& grid, std::vector<std::vector<std::vector<Direction>>>& visited, const size_t& y, const size_t& x, const Direction& dir)
{
    if(std::find(visited[y][x].begin(), visited[y][x].end(), dir) != visited[y][x].end())
        return;

    visited[y][x].push_back(dir);

    std::pair<Direction, Direction> next_direction{dir, none};

    if(grid[y][x] != '.') next_direction = change_dir(grid[y][x], dir);

    if(next_direction.first != none)
    {
        auto [next_y, next_x] = direction_to_coords(y, x, next_direction.first);
        if(next_y < grid.size() && next_x < grid[y].size()) calculate_light_path(grid, visited, next_y, next_x, next_direction.first);
    }

    if(next_direction.second != none)
    {
        auto [next_y, next_x] = direction_to_coords(y, x, next_direction.second);
        if(next_y < grid.size() && next_x < grid[y].size()) calculate_light_path(grid, visited, next_y, next_x, next_direction.second);
    }
}

size_t get_energy_sum(const std::vector<std::vector<std::vector<Direction>>>& visited)
{
    size_t result = 0;

    for(const auto& row : visited)
        for(const auto& visited : row)
            if(visited.size()) result++;

    return result;
}

int main(char argc, char* argv[])
{
    if(argc < 2) return 0;

    auto input = get_input(argv[1]);

    if(input == std::nullopt) return 1;

    std::vector<std::vector<std::vector<Direction>>> visited(input->size(), std::vector<std::vector<Direction>>(input->at(0).size()));

    size_t part_1_result = 0; 
    size_t part_2_result = 0;

    calculate_light_path(*input, visited, 0, 0, right);

    part_1_result = get_energy_sum(visited);

    for(size_t i = 0; i < input->size(); i++)
    {
        visited = std::vector<std::vector<std::vector<Direction>>>(input->size(), std::vector<std::vector<Direction>>(input->at(0).size())); // reset visited 
        calculate_light_path(*input, visited, i, 0, right);
        part_2_result = std::max(part_2_result, get_energy_sum(visited));

        visited = std::vector<std::vector<std::vector<Direction>>>(input->size(), std::vector<std::vector<Direction>>(input->at(0).size()));
        calculate_light_path(*input, visited, i, input->at(0).size() - 1, left);
        part_2_result = std::max(part_2_result, get_energy_sum(visited));
    }

    for(size_t i = 0; i < input->at(0).size(); i++)
    {
        visited = std::vector<std::vector<std::vector<Direction>>>(input->size(), std::vector<std::vector<Direction>>(input->at(0).size()));
        calculate_light_path(*input, visited, 0, i, down);
        part_2_result = std::max(part_2_result, get_energy_sum(visited));

        visited = std::vector<std::vector<std::vector<Direction>>>(input->size(), std::vector<std::vector<Direction>>(input->at(0).size()));
        calculate_light_path(*input, visited, input->at(0).size() - 1, i, up);
        part_2_result = std::max(part_2_result, get_energy_sum(visited));
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
