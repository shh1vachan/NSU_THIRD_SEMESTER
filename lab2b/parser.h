#pragma once
#include <vector>
#include <iostream>
#include <string>

class parser 
{
private:
    bool mode;                   
    std::size_t iterations;      
    std::string input_file;      
    std::string output_file;       
    
    std::string command;
    std::vector<std::string> command_params;

public:
    parser();

    static void parse_rule(const std::string& rule, std::vector<int>& neighbors_to_born, std::vector<int>& neighbors_to_survive);
  
    bool parse(int argc, char* argv[]);

    static void get_help();

    [[nodiscard]] std::size_t get_iterations() const;
    [[nodiscard]] bool is_offline_mode() const;
    [[nodiscard]] std::string get_input_file() const;
    [[nodiscard]] std::string get_output_file() const;

    bool parse_interactive_command(const std::string& input);
};
