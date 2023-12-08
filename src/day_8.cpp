#include <algorithm>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <thread>

struct TreeNode
{
  std::string name;
  TreeNode* left;
  TreeNode* right;
};

void count_steps(std::string path, TreeNode* start_node, size_t& result)
{
  result = 0;
  TreeNode* current = start_node;
  while(current->name.back() != 'Z')
  {
    if(path[result % path.size()] == 'L')
      current = current->left;
    else
      current = current->right;
    result++;
  }
}

size_t find_lowest_common_denominator(std::vector<size_t> values)
{
  size_t max_value = 0;
  size_t result;
  for(auto value : values)
    if(max_value < value) max_value = value;
  
  for(result = max_value; ;result += max_value)
  {
    size_t mod = 0;
    for(const auto& value : values)
    {
      mod = result % value;
      if(mod) break;
    }
    if(!mod) break; 
  }

  return result;
}

int main(char argc, char* argv[])
{
  std::ifstream input_file;

  if(argc > 1)
    input_file.open(argv[1]);

  if(!input_file.is_open())
  {
    std::cout << "Failed to open input file" << std::endl;
    return 1;  
  }

  std::string path;
  std::vector<std::string> input; 

  std::getline(input_file, path);
  path.resize((path.find(' ') != std::string::npos) ? path.find(' ') : path.size());

  while(input_file.good())
  {
    input.emplace_back();

    std::getline(input_file, input.back());
    if(input.back() == "") input.pop_back();
  }

  input_file.close();

  std::unordered_map<std::string, TreeNode*> node_map;

  for(const auto& str : input)
  {
    std::string node_name(str.begin(), std::find(str.begin(), str.end(), ' '));  
    node_map[node_name] = new TreeNode({node_name, nullptr, nullptr});
  }

  for(const auto& str : input)
  {
    std::string node_name(str.begin(), std::find(str.begin(), str.end(), ' '));                             // >KHH< = (CQL, HQF)
    std::string left(++std::find(str.begin(), str.end(), '('), std::find(str.begin(), str.end(), ','));     // KHH = (>CQL<, HQF)
    std::string right(++(++std::find(str.begin(), str.end(), ',')), std::find(str.begin(), str.end(), ')'));// KHH = (CQL, >HQF<)
    node_map[node_name]->left = node_map[left];
    node_map[node_name]->right = node_map[right];
  }

  // part 1 solution
  size_t part_1_result = 0;
  for(TreeNode* temp = node_map["AAA"]; temp->name != "ZZZ"; )
  {
    if(path[part_1_result % path.size()] == 'L')
      temp = temp->left;
    else
      temp = temp->right;
    part_1_result++;
  }

  // part 2 solution
  size_t part_2_result = 0;
  std::vector<TreeNode*> start_nodes;
  std::vector<size_t> results;
  std::vector<std::thread> threads;
  for(const auto& [name, ptr] : node_map)
  {
    if(name.back() == 'A') 
    {
      start_nodes.push_back(ptr);
      results.emplace_back(0);
    }
  }

  for(size_t i = 0; i < start_nodes.size(); i++)
    threads.emplace_back(count_steps, path, start_nodes[i], std::ref(results[i]));
  
  for(auto& thread : threads)
    thread.join();

  part_2_result = find_lowest_common_denominator(results);

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
