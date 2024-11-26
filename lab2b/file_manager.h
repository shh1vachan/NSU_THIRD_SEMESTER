#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "cell.h"
#include <iostream>
#include <vector>
#include <string>

class file_manager 
{
public:
    std::vector<std::vector<cell>> field;
    std::string field_name;
    std::string rule;
    int field_size = 20;

    friend std::istream& operator>>(std::istream& is, file_manager& manager);

    friend std::ostream& operator<<(std::ostream& os, const file_manager& manager);
};

#endif // FILE_MANAGER_H
