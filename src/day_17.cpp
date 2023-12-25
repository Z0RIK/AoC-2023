#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <unordered_set>

#define ID(y, x, dir, straight) size_t(y) << 16 * 3 | size_t(x) << 16 * 2 | size_t(dir) << 16 * 1 | uint16_t(straight)

enum direction
{
    none = 0,
    right = 0x1 << 2, 
    left = (0x1 << 2) + 1, 
    down = 0x1 << 3,
    up = (0x1 << 3) + 1
}; 

struct node{
    size_t y;
    size_t x;
    size_t distance;
    direction dir;
    uint32_t loss;
    uint32_t straight;
};

std::pair<size_t, size_t> coords_from_dir(const size_t& y, const size_t& x, const direction& dir)
{
    switch (dir)
    {
        case right:
            return {y, x + 1};
        case left:
            return {y, x - 1};
        case down:
            return {y + 1, x};
        case up:
            return {y - 1, x};
    }
    return {y, x};
}

size_t do_the_dijkstra(const std::vector<std::vector<uint8_t>>& input, 
                        size_t y_start, size_t x_start, 
                        size_t y_end, size_t x_end, 
                        size_t min_straigt, size_t max_straight)
{
    // directions for all neighbouring nodes
    std::vector<direction> neighbours({right, left, down, up});

    // create queue for not visited nodes
    auto node_cmp = [](const node& left, const node& right) -> bool { return left.distance > right.distance; };    // custom comparator
    std::priority_queue<node, std::vector<node>, decltype(node_cmp)> q(node_cmp);
    q.push({y_start, x_start, 0, none, input[y_start][x_start], 0});

    // set for visited nodes
    std::unordered_set<size_t> visited;

    while(!q.empty())
    {
        node current = q.top(); 
        q.pop();

        if(current.y == y_end && current.x == x_end && current.straight >= min_straigt) return current.distance; // arrived to destination

        if(visited.count(ID(current.y, current.x, current.dir, current.straight))) continue; // already visited this node
        visited.insert(ID(current.y, current.x, current.dir, current.straight)); // add current to visited set

        if(current.straight < max_straight && current.dir != none)
        {
            auto [next_y, next_x] = coords_from_dir(current.y, current.x, current.dir);
            if(next_y < input.size() && next_x < input[0].size()) 
                q.push({next_y, next_x, current.distance + input[next_y][next_x], current.dir, input[next_y][next_x], current.straight + 1});
        }

        if(current.straight >= min_straigt || current.dir == none)
        {
            for(auto dir : neighbours)
            {
                if(dir == current.dir || dir == direction(current.dir ^ 0x1))
                    continue;
                auto [next_y, next_x] = coords_from_dir(current.y, current.x, dir);
                if(next_y < input.size() && next_x < input[0].size()) 
                    q.push({next_y, next_x, current.distance + input[next_y][next_x], dir, input[next_y][next_x], 1});
            }
        }
    }

    return size_t(-1); 
}

int main(char argc, char* argv[])
{
    if(argc < 2) return 0;

    std::ifstream input_file(argv[1]);
    std::vector<std::vector<uint8_t>> input;

    if(input_file.is_open())
    {
        while(input_file.good())
        {
            std::string current_row;
            std::getline(input_file, current_row);
            if(current_row.size())
            {
                input.emplace_back();
                for(const auto& c : current_row)
                    input.back().emplace_back(c - '0');
            }
        }
    }
    else
    {
        std::cerr << "Failed to open input file" << std::endl;
    }

    size_t part_1_result = do_the_dijkstra(input, 0, 0, input.size() - 1, input[0].size() - 1, 0, 3);
    size_t part_2_result = do_the_dijkstra(input, 0, 0, input.size() - 1, input[0].size() - 1, 4, 10);

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

}
