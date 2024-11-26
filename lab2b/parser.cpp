#include "parser.h"
#include <cstring>
#include <iostream>
#include <sstream>

parser::parser() : iterations(0), mode(false) {}

bool parser::is_offline_mode() const { return mode; }
std::size_t parser::get_iterations() const { return iterations; }
std::string parser::get_input_file() const { return input_file; }
std::string parser::get_output_file() const { return output_file; }


//command line flags processing
bool parser::parse(int argc, char* argv[]) 
{
    for (size_t i = 1; i < argc; i++) 
    {
        const char* current_arg = argv[i];

        if (strcmp(current_arg, "-f") == 0 || strcmp(current_arg, "--file") == 0) 
        {
            if (i + 1 < argc) 
                input_file = std::string(argv[++i]);
            
            else 
            {
                std::cerr << "Error: Missing input filename. Use -h or --help for usage." << std::endl;
                return false;
            }
        }

        else if (strcmp(current_arg, "-o") == 0 || strcmp(current_arg, "--output") == 0) 
        {
            if (i + 1 < argc) 
                output_file = std::string(argv[++i]);
            
            else 
            {
                std::cerr << "Error: Missing output filename. Use -h or --help for usage." << std::endl;
                return false;
            }
            mode = true;  
        }

        else if (strcmp(current_arg, "-i") == 0 || strcmp(current_arg, "--iterations") == 0) 
        {
            if (i + 1 < argc) 
            {
                try 
                {
                    iterations = std::stoul(argv[++i]);
                } 
                catch (const std::invalid_argument&) 
                {
                    std::cerr << "Error: Invalid value for iterations. Use -h or --help for usage." << std::endl;
                    return false;
                }
            } 
            else 
            {
                std::cerr << "Error: Missing iterations value. Use -h or --help for usage." << std::endl;
                return false;
            }
            mode = true;  
        }
        else if (strcmp(current_arg, "-m") == 0 || strcmp(current_arg, "--mode") == 0) 
        {
            if (i + 1 < argc) 
            {
                std::string input_mode = std::string(argv[++i]);
                if (input_mode == "offline") 
                    mode = true; 
                
                else if (input_mode == "online") 
                {
                    mode = false;  

                    if (output_file.empty()) 
                        output_file = "none";  
                } 
                else 
                {
                    std::cerr << "Error: Invalid mode. Use 'online' or 'offline'. Use -h or --help for usage." << std::endl;
                    return false;
                }
            } 
            else 
            {
                std::cerr << "Error: Missing mode. Use -h or --help for usage." << std::endl;
                return false;
            }
        }
        else 
        {
            std::cerr << "Error: Unknown option " << current_arg << ". Use -h or --help for usage." << std::endl;
            return false;
        }
    }

    if (input_file.empty()) 
        input_file = "none";

    if (output_file.empty()) 
        output_file = "none";

    return true;
}


void parser::parse_rule(const std::string& rule, std::vector<int>& neighbors_to_born, std::vector<int>& neighbors_to_survive) 
{
    neighbors_to_born.clear();
    neighbors_to_survive.clear();

    if (rule[0] != 'B') 
    {
        throw std::invalid_argument("Invalid rule format: missing 'B'");
    }

    size_t slash_pos = rule.find('/');
    if (slash_pos == std::string::npos || rule[slash_pos + 1] != 'S') 
    {
        throw std::invalid_argument("Invalid rule format: missing 'S'");
    }

    for (size_t i = 1; i < slash_pos; ++i) 
    {
        if (std::isdigit(rule[i])) 
        {
            int neighbor_count = rule[i] - '0';
            if (neighbor_count > 8) 
            {
                throw std::invalid_argument("Invalid rule format: neighbors cannot exceed 8");
            }
            neighbors_to_born.push_back(neighbor_count);
        }
    }

    for (size_t i = slash_pos + 2; i < rule.size(); ++i) 
    {
        if (std::isdigit(rule[i])) 
        {
            int neighbor_count = rule[i] - '0';
            if (neighbor_count > 8) 
            {
                throw std::invalid_argument("Invalid rule format: neighbors cannot exceed 8");
            }
            neighbors_to_survive.push_back(neighbor_count);
        }
    }
}


//online mode commands processing
bool parser::parse_interactive_command(const std::string& input) 
{
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) 
        tokens.push_back(token);

    if (tokens.empty()) 
        return false;

    command = tokens[0];
    command_params.clear();
    if (tokens.size() > 1) 
        command_params.assign(tokens.begin() + 1, tokens.end());

    return true;
}
