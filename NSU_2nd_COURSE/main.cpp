
#include "data_writer.h"
#include "file_processing.h"
#include <iostream>
using namespace std;


int main(int argc, char** argv)
{
    if (argc != 3)
    {
        cerr << "Wrong arguments" << endl;
        return 1;
    }

    string input_filename = argv[1];
    string output_filename = argv[2];

    file_processing processor(input_filename);
    processor.extract_from_txt();

    data_writer writer(output_filename);

    //add data from object processor to object writer
    for (const auto& entry : processor.get_data())
    {
        writer.add_data(std::get<0>(entry), std::get<1>(entry), std::get<2>(entry));
    }

    writer.write_to_csv();

    return 0;
}