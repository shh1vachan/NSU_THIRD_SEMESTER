#pragma once

#include <tuple>
#include <string>

template<typename char_type, typename traits_type, typename Tuple, size_t N>
class printer
{
public:
    static void print(std::basic_ostream<char_type, traits_type> &output_stream, const Tuple &tuple)
    {
        printer<char_type, traits_type, Tuple, N - 1>::print(output_stream, tuple);
        output_stream << ";" << std::get<N - 1>(tuple);
    }
};

template<typename char_type, typename traits_type, typename Tuple>
class printer<char_type, traits_type, Tuple, 1>
{
public:
    static void print(std::basic_ostream<char_type, traits_type> &output_stream, const Tuple &tuple)
    {
        output_stream << std::get<0>(tuple);
    }
};

template<typename char_type, typename traits_type, typename... Args>
std::basic_ostream<char_type, traits_type> &operator<<(std::basic_ostream<char_type, traits_type> &output_stream, const std::tuple<Args...> &tuple)
{
    output_stream << "{";
    printer<char_type, traits_type, decltype(tuple), sizeof...(Args)>::print(output_stream, tuple);
    output_stream << "}";
    return output_stream;
}
