#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "cell.h"
#include "file_manager.h"
#include "parser.h"

class Game 
{
public:

    Game(int field_size);
    ~Game();

    //main funcs 
    bool prepare_game(const parser& parser, file_manager& manager);        
    void run();                                     
    void run_iterations(int n);                     
    void display() const;                           
    void save_to_file(const std::string& filename); 

    bool is_valid_rule(const std::string& rule);

    //helper func
    //in public to give access to tests
    bool execute_command(const std::string& command); 



private:
    int field_size;                                 
    int current_iteration;                         
    std::string universe_name;                      
    std::string rule;                              
    std::vector<std::vector<cell>> game_field; 
    std::vector<int> neighbors_to_born;
    std::vector<int> neighbors_to_survive;     

    file_manager manager;

    //helper funcs   
    void randomized_universe();                
    void run_iteration();                    
    int count_alive_neighbors(int x, int y) const;  
    

    //declare friend classes for tests
    friend class GameTests_RandomUniverseGeneration_Test;
    friend class GameTests_RunIteration_Test;

};

#endif // GAME_H
