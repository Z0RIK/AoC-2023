#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

enum class direction{
    none    = 0,
    east    = (1 << 2),
    south   = (1 << 3),
    west    = (1 << 2) + 1, 
    north   = (1 << 3) + 1,
    count
};

direction get_direction(direction from, char pipe)
{
    switch (pipe)
    {
        case '|':
            switch (from)
            {
                case direction::north:
                    return direction::south;
                case direction::south:
                    return direction::north;
            }
            break;
        case '-':
            switch (from)
            {
                case direction::east:
                    return direction::west;
                case direction::west:
                    return direction::east;
            }
            break;
        case 'L':
            switch (from)
            {
                case direction::east:
                    return direction::north;
                case direction::north:
                    return direction::east;
            }
            break;
        case 'J':
            switch (from)
            {
                case direction::west:
                    return direction::north;
                case direction::north:
                    return direction::west;
            }
            break;
        case '7':
            switch (from)
            {
                case direction::west:
                    return direction::south;
                case direction::south:
                    return direction::west;
            }
            break;
        case 'F':
            switch (from)
            {
                case direction::east:
                    return direction::south;
                case direction::south:
                    return direction::east;
            }
            break;
        default:
            break;
    }
    return direction::none;
}

inline std::pair<int, int> direction_to_offset(direction dir)
{
    switch (dir)
    {
        case direction::east: 
            return {0, 1};
        case direction::south: 
            return {1, 0};
        case direction::west:
            return {0, -1};
        case direction::north: 
            return {-1, 0};
        default:
            return {0, 0};
    }
}

size_t find_loop_length(const std::vector<std::string>& input, size_t y, size_t x, size_t step, direction from)
{
    if(input[y][x] == 'S') return step; 

    direction to = get_direction(from, input[y][x]);
    auto [offset_y, offset_x] = direction_to_offset(to);

    //std::cout << input[y][x] << "->"; 

    if(!(!offset_y && !offset_x) && (size_t(offset_y + y) < input.size() && size_t(offset_x + x) < input[offset_y + y].size()))
    {
        return find_loop_length(input, offset_y + y, offset_x + x, step + 1, static_cast<direction>(static_cast<uint8_t>(to) ^ 0x1));
    }

    return 0;
}

void mart_right_side(size_t y, size_t x, direction dir, std::vector<std::string>& visited)
{
    direction right_side_direction = direction::none;
    switch (dir)
    {
        case direction::east:
            right_side_direction = direction::north;
            break;
        case direction::north:
            right_side_direction = direction::west;
            break;
        case direction::west:
            right_side_direction = direction::south;
            break;
        case direction::south:
            right_side_direction = direction::east;
            break;
    }
    auto [offset_y, offset_x] = direction_to_offset(right_side_direction);

    y = size_t(y + offset_y);
    x = size_t(x + offset_x);

    if(y < visited.size() && x < visited[y].size() && visited[y][x] != 'P') visited[y][x] = 'R';
}

void mark_path(const std::vector<std::string>& input, size_t y, size_t x, direction from, std::vector<std::string>& visited)
{
    visited[y][x] = 'P'; // mark path with P

    if(input[y][x] == 'S') return;

    //std::cout << input[y][x] << "->"; 

    direction to = get_direction(from, input[y][x]);
    auto [offset_y, offset_x] = direction_to_offset(to);

    mart_right_side(y, x, from, visited);
    mart_right_side(y, x, static_cast<direction>(static_cast<uint8_t>(to) ^ 0x1), visited);
    
    mark_path(input, offset_y + y, offset_x + x, static_cast<direction>(static_cast<uint8_t>(to) ^ 0x1), visited);
}

void fill_right_side(size_t y, size_t x, std::vector<std::string>& visited)
{
    visited[y][x] = 'R';
    // down
    if(y + 1 < visited.size() && visited[y + 1][x] == '0') fill_right_side(y + 1, x, visited);
    // up
    if(size_t(y - 1) < visited.size() && visited[y - 1][x] == '0') fill_right_side(y - 1, x, visited);
    // right
    if(x + 1 < visited[y].size() && visited[y][x + 1] == '0') fill_right_side(y, x + 1, visited);
    // left
    if(size_t(x - 1) < visited[y].size() && visited[y][x - 1] == '0') fill_right_side(y, x - 1, visited);
}

int main(char argc, char* argv[])
{
    std::ifstream input_file;

    if(argc < 2) return 0;

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

    size_t y{}, x{};

    for(y = 0; y < input.size(); y++)
    {
        for(x = 0; x < input[y].size(); x++)
            if(input[y][x] == 'S') break;
        if(input[y][x] == 'S') break;
    }

    direction start_direction = direction::none;
    size_t part_1_result = 0;
    size_t length = 0;

    if(y) 
    {
        start_direction = direction::north;
        length = std::max(length, find_loop_length(input, y - 1, x, 1, direction::south));
    }
    if(x && !length) 
    {
        start_direction = direction::west;
        length = std::max(length, find_loop_length(input, y, x - 1, 1, direction::east));
    }
    if(y + 1 < input.size() && !length)
    {
        start_direction = direction::south;
        length = std::max(length, find_loop_length(input, y + 1, x, 1, direction::north));
    }
    if(x + 1 < input[y].size() && !length) 
    {
        start_direction = direction::east;
        length = std::max(length, find_loop_length(input, y, x + 1, 1, direction::west));
    }
    part_1_result = length / 2 + (length & 0x1);

    size_t part_2_result = 0;
    std::vector<std::string> visited(input.size(), std::string(input[0].size(), '0'));

    mark_path(input, y + direction_to_offset(start_direction).first, x + direction_to_offset(start_direction).second, static_cast<direction>(static_cast<uint8_t>(start_direction) ^ 0x1), visited);
    for(size_t y = 0; y < visited.size(); y++)
        for(size_t x = 0; x < visited[y].size(); x++)
            if(visited[y][x] == 'R') fill_right_side(y, x, visited);

    char outside = 'R';
    size_t left_side_size = 0;

    for(size_t y = 0; y < visited.size(); y++)
    {
        for(size_t x = 0; x < visited[y].size(); x++)
            if(visited[y][x] == '0')
            {
                visited[y][x] = 'L';
                if(y == 0 || x == 0 || y + 1 == visited.size() || x + 1 == visited[y].size()) outside = 'L';
                left_side_size++;
            }
        //std::cout << visited[y] << std::endl;
    }

    if(outside == 'R')
        part_2_result = left_side_size;
    else
        part_2_result = visited.size() * visited[0].size() - length - left_side_size;

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
