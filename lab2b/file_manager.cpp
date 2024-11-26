#include "file_manager.h"
#include <iostream>
#include <sstream>

std::istream& operator>>(std::istream& is, file_manager& manager) 
{
    std::string line;
    bool rule_found = false, name_found = false;


    manager.field = std::vector<std::vector<cell>>(manager.field_size, std::vector<cell>(manager.field_size));

    if (!std::getline(is, line)) //first line
    {
        std::cerr << "Error: empty or incorrect input" << std::endl;
        return is;
    }

    if (line != "#Life 1.06") 
        std::cerr << "Warning: incorrect input format, expected '#Life 1.06' in the first line." << std::endl;

    while (std::getline(is, line)) //other lines
    {
        if (line.empty()) 
            continue;

        if (line.rfind("#N ", 0) == 0) 
        {
            manager.field_name = line.substr(3); //remove #N
            name_found = true;
        }

        else if (line.rfind("#R ", 0) == 0) 
        {
            manager.rule = line.substr(3); //remove #R
            rule_found = true;
        }

        else 
        {
            std::istringstream iss(line);
            int x, y;
            if (!(iss >> x >> y)) 
            {
                std::cerr << "Warning: invalid format for coordinates: " << line << std::endl;
                continue;
            }

            if (y >= 0 && y < manager.field.size() && x >= 0 && x < manager.field[0].size()) 
                manager.field[y][x].set_current_state(true); 
            
            else 
                std::cerr << "Warning: coordinates out of bounds: " << x << ", " << y << std::endl;
        }
    }

    if (!name_found) 
        std::cerr << "Warning: universe name not found in the input." << std::endl;

    if (!rule_found) 
        std::cerr << "Warning: transition rule not found in the input." << std::endl;

    return is;
}


std::ostream& operator<<(std::ostream& os, const file_manager& manager) 
{
    os << "#Life 1.06" << std::endl;
    os << "#N " << manager.field_name << std::endl;
    os << "#R " << manager.rule << std::endl;

    for (size_t y = 0; y < manager.field.size(); ++y) 
        for (size_t x = 0; x < manager.field[y].size(); ++x) 
            if (manager.field[y][x].is_alive()) 
                os << x << ' ' << y << std::endl;
    
    return os;
}
