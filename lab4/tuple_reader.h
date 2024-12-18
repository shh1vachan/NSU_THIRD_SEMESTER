#pragma once

#include <tuple>
#include <sstream>
#include <stdexcept>
#include "errors.h"

template<typename char_type, typename traits_type, typename first, typename... last>
class reader
{
public:
    static std::tuple<first, last...> read(std::basic_istream<char_type, traits_type> &input_stream, size_t line_num, size_t col_num = 1)
    {
        if (input_stream.eof())
        {
            throw NotEnoughData{line_num};
        }
        first data;

        if (!(input_stream >> data))
        {
            if (input_stream.eof())
            {
                return {}; 
            }
            throw FailedToReadData{line_num, col_num}; 
        }
        return std::tuple_cat(std::make_tuple(data), reader<char_type, traits_type, last...>::read(input_stream, line_num, col_num + 1));
    }
};

template<typename char_type, typename traits_type, typename first>
class reader<char_type, traits_type, first>
{
public:
    static std::tuple<first> read(std::basic_istream<char_type, traits_type> &input_stream, size_t line_num, size_t col_num = 1)
    {
        if (input_stream.eof())
        {
            throw NotEnoughData{line_num};
        }
        first data;
        if (!(input_stream >> data))
        {
            if (input_stream.eof())
            {
                return {}; 
            }
            throw FailedToReadData{line_num, col_num}; 
        }
        return std::make_tuple(data);
    }
};
