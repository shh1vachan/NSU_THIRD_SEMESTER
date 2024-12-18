#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "tuple_printer.h"
#include "csv_parser.h"
#include "errors.h"


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./lab4 <number_of_lines_to_skip> <csv_file_name>" << std::endl;
        return 1;
    }

    size_t skipped_lines = std::stoi(argv[1]);
    std::string file_name = argv[2];

    std::ifstream file{ file_name };

    if (file.fail())
    {
        throw FileOpenError();
    }

    parser<size_t, std::string, int, std::string, double> parser{ file, skipped_lines };

    try
    {
        for (auto tuple : parser)
        {
            //skip empty lines
            if (std::get<0>(tuple) == 0 && std::get<1>(tuple).empty())
            {
                continue;
            }
            std::cout << tuple << std::endl;
        }
    }
    //catch possible errors
    catch (NotEnoughData& e)
    {
        std::cerr << e.what() << " Line: " << e.line_number() << "." << std::endl;
    }
    catch (FailedToReadData& e)
    {
        std::cerr << e.what() << " Line: " << e.line_number() << ". Column: " << e.col_number() << "." << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
    }

    file.close();
    return 0;
}
