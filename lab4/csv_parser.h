#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <locale>
#include "tuple_reader.h"
#include "errors.h"

template <typename... Args>
class parser
{
public:
    //parser configuration
    parser(std::ifstream &file, size_t skipped_lines = 0, char separator = ';', char escaping_symbol = '"');

    class Iterator
    {
    public:
        Iterator(parser &parser, bool is_end);
        ~Iterator() = default;

        bool operator!=(const Iterator &other) const;
        Iterator &operator++();
        std::tuple<Args...> operator*() const;

    private:
        parser &_parser;
        std::tuple<Args...> _tuple_cur;
        bool _is_end;
        size_t _line_num;
        std::streampos _cur_pos;

        class CSV_classifier : public std::ctype<char>
        {
        private:
            mask _table[table_size];

        public:
            explicit CSV_classifier(char separator, size_t refs = 0);
        };

        void read_tuple();
        std::string parse_CSV(const std::string &input);
    };

    Iterator begin();
    Iterator end();

private:
    size_t _skipped_lines;
    std::ifstream &_file;
    char _separator;
    char _escaping_symbol;
    std::streampos _start_pos;

    void skip_lines();
};
