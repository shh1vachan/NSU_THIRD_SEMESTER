#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <string>


class data_writer{
private:
    std::string filename_;
    std::vector<std::tuple<std::string, int, float>>data_;

public:
    data_writer(const std::string& filename);

    void add_data(const std::string& name, int int_value, float float_value);
    void write_to_csv() const;
};
