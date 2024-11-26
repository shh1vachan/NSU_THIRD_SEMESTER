#include "game.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>   // for std::rand и std::srand
#include <ctime>     // for std::time
#include <algorithm> //for std::find


Game::Game(int field_size)
    : field_size(field_size), current_iteration(0), game_field(field_size, std::vector<cell>(field_size)) {}

Game::~Game() = default;


bool Game::is_valid_rule(const std::string& rule) 
{
    if (rule[0] != 'B' || rule.find('/') == std::string::npos)
        return false;
    size_t slash_pos = rule.find('/');

    if (slash_pos + 1 >= rule.size() || rule[slash_pos + 1] != 'S')
        return false;

    for (size_t i = 1; i < slash_pos; ++i)
        if (rule[i] < '0' || rule[i] > '8')
            return false;

    for (size_t i = slash_pos + 2; i < rule.size(); ++i)
        if (rule[i] < '0' || rule[i] > '8')
            return false;

    return true;
}

bool Game::prepare_game(const parser& cmd_parser, file_manager& manager) 
{
    std::vector<int> neighbors_to_born;
    std::vector<int> neighbors_to_survive;

    if (cmd_parser.is_offline_mode()) 
    {
        if (cmd_parser.get_input_file() == "none") 
        {
            std::cerr << "Error: no input file provided." << std::endl;
            return false;
        } 
        else 
        {
            std::ifstream in_file(cmd_parser.get_input_file());
            if (!in_file.is_open()) 
            {
                std::cerr << "Error: Failed to open file " << cmd_parser.get_input_file() << std::endl;
                return false;
            }

            in_file >> manager;
            game_field = manager.field;
            universe_name = manager.field_name;
            rule = manager.rule;

            try 
            {
                parser::parse_rule(rule, neighbors_to_born, neighbors_to_survive);
            } 

            catch (const std::invalid_argument& e) 
            {
                std::cerr << "Error: " << e.what() << std::endl;
                return false;
            }
        }

        if (cmd_parser.get_output_file() == "none") 
        {
            std::cerr << "Error: No output file specified in offline mode." << std::endl;
            return false;
        }
    } 
    else 
    {
        if (cmd_parser.get_input_file() == "none") 
        {
            std::cout << "No input file provided. Generating random universe in interactive mode." << std::endl;
            randomized_universe();
            universe_name = "Random Universe";
            rule = "B3/S23";
            try 
            {
                parser::parse_rule(rule, neighbors_to_born, neighbors_to_survive);
            } 
            catch (const std::invalid_argument& e) 
            {
                std::cerr << "Error: " << e.what() << std::endl;
                return false;
            }
        } 
        else 
        {
            std::ifstream in_file(cmd_parser.get_input_file());
            if (!in_file.is_open()) 
            {
                std::cerr << "Error: Failed to open file " << cmd_parser.get_input_file() << std::endl;
                return false;
            }

            in_file >> manager;
            game_field = manager.field;
            universe_name = manager.field_name;
            rule = manager.rule;

            try 
            {
                parser::parse_rule(rule, neighbors_to_born, neighbors_to_survive);
            } 
            catch (const std::invalid_argument& e) 
            {
                std::cerr << "Error: " << e.what() << std::endl;
                return false;
            }
        }
    }

    if (cmd_parser.get_iterations() <= 0 && cmd_parser.is_offline_mode()) 
    {
        std::cerr << "Error: Number of iterations must be greater than zero in offline mode." << std::endl;
        return false;
    }

    std::cout << "Loaded universe: " << universe_name << std::endl;
    std::cout << "Rule: " << rule << std::endl;
    std::cout << "Game prepared successfully!" << std::endl;

    // save conditions
    this->neighbors_to_born = neighbors_to_born;
    this->neighbors_to_survive = neighbors_to_survive;

    return true;
}


void Game::randomized_universe() 
{
    std::srand(std::time(0));
    for (std::size_t x = 0; x < field_size; x++)
        for (std::size_t y = 0; y < field_size; y++)
            game_field[x][y].set_current_state(std::rand() % 2 == 0);
}


void Game::run() 
{
    std::string command;
    while (true) 
    {
        std::cout << "Enter command (tick, dump, exit, help): ";
        std::getline(std::cin, command);
        if (!execute_command(command))
            break;
    }
}


bool Game::execute_command(const std::string& command) 
{
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "tick" || cmd == "t") 
    {
        int iterations = 1;
        if (iss >> iterations)
            run_iterations(iterations);
        else
            run_iterations(1);
        display();
    }
    else if (cmd == "dump") 
    {
        std::string filename;
        if (iss >> filename) 
        {
            save_to_file(filename); 
        }
        else 
            std::cerr << "Error: No filename provided for dump command." << std::endl;
    }

    else if (cmd == "exit") 
        return false;

    else if (cmd == "help") 
    {
        std::cout << "Usage: <program executable file> [options]" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "-f, --file FILENAME\t\tInput file with the universe data." << std::endl;
        std::cout << "-o, --output FILENAME\t\tOutput file to save the universe." << std::endl;
        std::cout << "-i, --iterations NUMBER\t\tNumber of iterations to run in offline mode." << std::endl;
        std::cout << "-m, --mode MODE\t\t\tMode of the game ('online' or 'offline')." << std::endl;
        std::cout << "-h, --help\t\t\tDisplay this help message." << std::endl;
        std::cout << "Interactive commands after startup:" << std::endl;
        std::cout << "dump <filename>\t\t\tSave the universe to a file." << std::endl;
        std::cout << "tick <n=1>\t\t\tCalculate and display the next n iterations." << std::endl;
        std::cout << "t <n=1>\t\t\t\tShort form of tick command." << std::endl;
        std::cout << "exit\t\t\t\tExit the game." << std::endl;
        std::cout << "help\t\t\t\tShow help for interactive commands." << std::endl;
    }

    else 
        std::cerr << "Error: Unknown command." << std::endl;
    
    return true;
}


void Game::run_iterations(int n) 
{
    for (int i = 0; i < n; ++i)
    {
        run_iteration();
    }
}


void Game::run_iteration() 
{

    for (int x = 0; x < field_size; ++x) 
    {
        for (int y = 0; y < field_size; ++y) 
        {
            bool next_state;
            int alive_neighbors = count_alive_neighbors(x, y);

            if(game_field[x][y].is_alive()) 
            {
                next_state = std::find(neighbors_to_survive.begin(), neighbors_to_survive.end(), alive_neighbors) != neighbors_to_survive.end();
            } 
            else 
            {
                next_state = std::find(neighbors_to_born.begin(), neighbors_to_born.end(), alive_neighbors) != neighbors_to_born.end();
            }
            game_field[x][y].save_state(next_state);
        }
    }

    for (int x = 0; x < field_size; ++x) 
        for (int y = 0; y < field_size; ++y) 
            game_field[x][y].update_state();
    
    ++current_iteration;
}


int Game::count_alive_neighbors(int x, int y) const 
{
    int alive_neighbors = 0;
    for (int dx = -1; dx <= 1; ++dx) 
    {
        for (int dy = -1; dy <= 1; ++dy) 
        {
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + field_size) % field_size;
            int ny = (y + dy + field_size) % field_size;
            if (game_field[nx][ny].is_alive()) ++alive_neighbors;
        }
    }
    return alive_neighbors;
}


void Game::display() const 
{
    std::cout << "Universe: " << universe_name << ", Rule: " << rule << ", Iteration: " << current_iteration << std::endl;
    for (int x = 0; x < field_size; ++x) {
        for (int y = 0; y < field_size; ++y)
            std::cout << (game_field[x][y].is_alive() ? 'O' : '.');
        std::cout << '\n';
    }
}


void Game::save_to_file(const std::string& filename) 
{
    manager.field = game_field;
    manager.field_name = universe_name;
    manager.rule = rule;
    
    std::ofstream output_file(filename);
    if (output_file.is_open()) 
    {
        output_file << manager;
        std::cout << "Universe saved to " << filename << std::endl;
    } 
    else 
        std::cerr << "Cannot open file: " << filename << std::endl;
}
