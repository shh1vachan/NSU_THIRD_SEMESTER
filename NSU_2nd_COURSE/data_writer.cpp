#include "data_writer.h"
#include <fstream>


data_writer::data_writer(const std::string& filename) : filename_(filename) {}

void data_writer::add_data(const std::string& name, int int_value, float float_value)
{
    data_.emplace_back(name, int_value, float_value);
}

void data_writer::write_to_csv() const
{
    std::ofstream file(filename_);

    if(!file.is_open())
    {
        std::cerr << "cannot open CSV file: " << filename_ << std::endl;
        return;
    }

    //write headline and data tuples to .csv file
    file << "слово,частота,частота(%)\n";

    for (const auto& entry : data_)
    {
        file << std::get<0>(entry) << "," << std::get<1>(entry) << "," << std::get<2>(entry) << "\n";
    }
}
