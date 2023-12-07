#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

enum class hand_type{
    five_of_a_kind  = 6,
    four_of_a_kind  = 5,
    full_house      = 4,
    three_of_a_kind = 3,
    two_pair        = 2,
    one_pair        = 1,
    high_card       = 0
};

static std::unordered_map<char, uint8_t> label_power = {
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'T', 10},
    {'J', 11},
    {'Q', 12},
    {'K', 13},
    {'A', 14}
};

static std::vector<char> labels = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

hand_type determine_hand_type(std::string hand)
{
    static std::unordered_map<char, size_t> hash;
    hash.clear();

    for(const auto& c: hand)
        hash[c]++;

    switch(hash.size()) // hash.size() determines amount of unique card labels in given hand
    {
        case 1: // only one unique label means all cards have same label
            return hand_type::five_of_a_kind;
        case 2: // either 4 + 1 or 2 + 3
            if(hash.begin()->second == 1 || (++hash.begin())->second == 1)
                return hand_type::four_of_a_kind;
            else
                return hand_type::full_house;
        case 3: // either 3 + 1 + 1 or 2 + 2 + 1
            for(const auto& [label, count]: hash)
                if(count == 3) return hand_type::three_of_a_kind;
            return hand_type::two_pair;
        case 4: // only one pair possible
            return hand_type::one_pair;
        default: // all cards are unique
            return hand_type::high_card;
            break;
    }
}

hand_type best_possible_hand_type(std::string hand)
{
    hand_type result = determine_hand_type(hand);
    static std::vector<size_t> joker_indexes;
    
    if(hand.find('J') != std::string::npos)
    {   
        joker_indexes.resize(0);
        for(size_t i = 0; i < 5; i++) 
            if(hand[i] == 'J') joker_indexes.push_back(i);
        
        for(const auto& label : labels)
        {
            for(const auto& index : joker_indexes)
                hand[index] = label;

            hand_type pretender_type = determine_hand_type(hand);

            if(pretender_type > result) result = pretender_type;
        }
        
    }

    return result;
}

bool hand_compare_p1(std::pair<std::string, size_t> left, std::pair<std::string, size_t> right)
{
    hand_type l = determine_hand_type(left.first);
    hand_type r = determine_hand_type(right.first);
    if (l != r)
        return l < r;
    
    size_t i = 0;
    while(left.first[i] == right.first[i])
        i++;

    if(i < 5)
        return label_power[left.first[i]] < label_power[right.first[i]];
    else
        return false;
}

bool hand_compare_p2(std::pair<std::string, size_t> left, std::pair<std::string, size_t> right)
{
    hand_type l = best_possible_hand_type(left.first);
    hand_type r = best_possible_hand_type(right.first);

    if (l != r)
        return l < r;
    
    size_t i = 0;
    while(left.first[i] == right.first[i])
        i++;

    if(i < 5)
        return label_power[left.first[i]] < label_power[right.first[i]];
    else
        return false;
}

int main(char argc, char* argv[])
{
    std::ifstream input_file;
    if (argc > 1)
        input_file.open(argv[1]);

    if(!input_file.is_open())         
    {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    std::vector<std::string> input;
    std::vector<std::pair<std::string, size_t>> games;

    while(input_file.good())
    {
        input.emplace_back();
        std::getline(input_file, input.back());
    }

    // parsing input
    for(auto& str : input)
    {
        size_t card_end = str.find_first_of(' ');
        size_t bid = std::stoi(std::string(str.begin() + card_end, str.end()));
        str.resize(card_end);
        games.emplace_back(str, bid);
    }

    // part 1 solution
    size_t part_1_result = 0;
    std::sort(games.begin(), games.end(), hand_compare_p1);
    
    for(size_t i = 0; i < games.size(); i++)
        part_1_result += (i + 1) * games[i].second;

    // part 2 solution
    size_t part_2_result = 0;
    label_power['J'] = 0;
    std::sort(games.begin(), games.end(), hand_compare_p2);

    for(size_t i = 0; i < games.size(); i++)
        part_2_result += (i + 1) * games[i].second;

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
