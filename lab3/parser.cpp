#include "parser.h"
#include "exceptions.h"
#include <fstream>
#include <sstream>
#include <iostream>

ConfigParser::ConfigParser(const std::string& config_path, const std::vector<WAVFile>& additional_files)
    : config_path_(config_path), additional_files_(additional_files) {}


std::vector<std::unique_ptr<Converter>> ConfigParser::parse_config() 
{
    std::ifstream config_file(config_path_);
    if (!config_file) 
        throw FileOpenException(config_path_);

    std::vector<std::unique_ptr<Converter>> converters;
    std::string line;
    int line_number = 0;

    while (std::getline(config_file, line)) 
    {
        ++line_number;
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        try 
        {
            if (command == "mute") 
            {
                int start_time, end_time;
                iss >> start_time >> end_time;
                if (iss.fail()) 
                    throw InvalidConfigLineException(line_number, "Invalid parameters for mute command");
                
                converters.emplace_back(ConverterFactory::createConverter("mute", {start_time, end_time}));
            } 
            else if (command == "mix") 
            {
                std::string additional_file;
                int insert_time = 0;
                iss >> additional_file >> insert_time;
                if (iss.fail()) 
                    throw InvalidConfigLineException(line_number, "Invalid parameters for mix command");

                if (additional_file[0] == '$') 
                {
                    int file_index = std::stoi(additional_file.substr(1));
                    if (file_index < 2 || file_index > additional_files_.size() + 1) 
                        throw InvalidConfigLineException(line_number, "Invalid file index in mix command: " + std::to_string(file_index));
                    
                    const WAVFile& additional_input = additional_files_[file_index - 2];
                    converters.emplace_back(ConverterFactory::createConverter("mix", {insert_time}, &additional_input));
                } 
                else 
                    throw InvalidConfigLineException(line_number, "Mix command must reference a file index with $");
            } 
            else if (command == "compress") 
            {
                int max_amplitude;
                iss >> max_amplitude;
                if (iss.fail()) 
                    throw InvalidConfigLineException(line_number, "Invalid parameters for compress command");
                
                converters.emplace_back(ConverterFactory::createConverter("compress", {max_amplitude}));
            } 
            else 
                throw InvalidCommandException(command);
        } 
        catch (const std::exception& e) 
        {
            throw InvalidConfigLineException(line_number, e.what());
        }
    }
    return converters;
}

void ConfigParser::print_help() 
{
    std::cout << "Usage: ./sound_processor -c <config.txt> <output.wav> <input1.wav> [input2.wav ...]\n";
    std::cout << "Options:\n";
    std::cout << "  -c      Specify the configuration file\n";
    std::cout << "Arguments:\n";
    std::cout << "  config.txt       Configuration file with conversion instructions\n";
    std::cout << "  output.wav       Output WAV file\n";
    std::cout << "  input1.wav       Primary input WAV file\n";
    std::cout << "  input2.wav ...   Additional input WAV files for mixing (optional)\n";

    std::cout << "\nSupported converters:\n";

    const std::vector<std::pair<std::string, std::string>> converters = 
    {
        {"mute", "mute <start_time> <end_time>: Mutes the audio between <start_time> and <end_time> seconds."},
        {"mix", "mix $<index> <insert_time>: Mixes the input file at index <index> with the primary input starting at <insert_time> seconds."},
        {"compress", "compress <max_amplitude>: Limits the amplitude of the audio to <max_amplitude>."}
    };

    for (const auto& [name, description] : converters) 
        std::cout << "  " << description << "\n";

    std::cout << "\nNotes:\n";
    std::cout << "  - Comments in the configuration file start with '#'.\n";
    std::cout << "  - Time is specified in seconds.\n";
    std::cout << "  - File indices in 'mix' command are 1-based (e.g., $2 references the second input file).\n";
}
