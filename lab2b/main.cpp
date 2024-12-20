#include <iostream>
#include "parser.h"
#include "game.h"
#include "file_manager.h"

int main(int argc, char* argv[]) 
{
    //start commands processing
    parser cmd_parser;
    if (!cmd_parser.parse(argc, argv)) 
    {
        std::cerr << "Error: Failed to parse command-line arguments." << std::endl;
        return 1;
    }


    int field_size = 20; //default field size, can be changed
    Game game(field_size);
    file_manager manager;
    manager.field_size = field_size; 

    if (!game.prepare_game(cmd_parser, manager)) 
    {
        std::cerr << "Error: Failed to prepare the game." << std::endl;
        return 1;
    }

    if (cmd_parser.is_offline_mode()) 
    {
        int iterations = cmd_parser.get_iterations();
        game.run_iterations(iterations);

        game.save_to_file(cmd_parser.get_output_file());
        std::cout << "Game completed in offline mode." << std::endl;
    }

    else 
    {
        std::cout << "Starting the game in interactive mode." << std::endl;
        game.run();
    }

    return 0;
}
