#include "csv_parser.h"

//explicit template instantiation
template class parser<unsigned long, std::string, int, std::string, double>;

template <typename... Args>
parser<Args...>::parser(std::ifstream &file, size_t skipped_lines, char separator, char escaping_symbol)
    : _file(file), _skipped_lines(skipped_lines), _separator(separator), _escaping_symbol(escaping_symbol)
{
    skip_lines();
    _start_pos = _file.tellg();
}

template <typename... Args>
parser<Args...>::Iterator::Iterator(parser &parser, bool is_end)
    : _parser(parser), _is_end(is_end), _line_num(parser._skipped_lines + 1), _cur_pos(parser._start_pos)
{
    if (is_end)
    {
        return;
    }
    read_tuple();
}

template <typename... Args>
bool parser<Args...>::Iterator::operator!=(const Iterator &other) const
{
    return _is_end != other._is_end;
}

template <typename... Args>
typename parser<Args...>::Iterator &parser<Args...>::Iterator::operator++()
{
    read_tuple();
    return *this;
}

template <typename... Args>
std::tuple<Args...> parser<Args...>::Iterator::operator*() const
{
    return _tuple_cur;
}

template <typename... Args>
parser<Args...>::Iterator::CSV_classifier::CSV_classifier(char separator, size_t refs)
    : std::ctype<char>{ &_table[0], false, refs }
{
    std::copy_n(classic_table(), table_size, _table);
    _table[' '] = lower;
    _table[separator] = space;
}

template <typename... Args>
void parser<Args...>::Iterator::read_tuple() 
{
    if (_parser._file.eof() || !_parser._file) 
    {
        _is_end = true;
        return;
    }

    _parser._file.clear();
    _parser._file.seekg(_cur_pos);

    std::string line_cur;
    std::getline(_parser._file, line_cur);
    _cur_pos = _parser._file.tellg();

    if (line_cur.empty() || _parser._file.eof()) 
    {
        _is_end = true;
        return;
    }

    try 
    {
        std::string result_line = parse_CSV(line_cur);
        std::istringstream line_parser(result_line);

        line_parser.imbue(std::locale(std::locale::classic(), new CSV_classifier(_parser._separator)));

        _tuple_cur = reader<char, std::char_traits<char>, Args...>::read(line_parser, _line_num);
        ++_line_num;
    } 
    catch (const FailedToReadData &e) 
    {
        std::cerr << e.what() << " Line: " << e.line_number() << ". Column: " << e.col_number() << "." << std::endl;
        ++_line_num;
        read_tuple();  
    }
}


template <typename... Args>
std::string parser<Args...>::Iterator::parse_CSV(const std::string &input)
{
    std::string result;
    bool inside_quotes = false;
    bool prev_escaped = false;

    for (size_t i = 0; i < input.size(); ++i)
    {
        char c = input[i];

        if (c == _parser._escaping_symbol && !prev_escaped)
        {
            inside_quotes = !inside_quotes;  
            prev_escaped = true; 
        }
        else if (c == _parser._escaping_symbol && prev_escaped)
        {
            result += c;  
            prev_escaped = false;  
        }

        else
        {
            result += c; 
            prev_escaped = false; 
        }
    }

    if (inside_quotes)
    {
        throw FailedToReadData(_line_num, input.size()); 
    }

    return result;
}

template <typename... Args>
parser<Args...>::Iterator parser<Args...>::begin()
{
    _file.clear();
    _file.seekg(_start_pos);
    return Iterator(*this, false);
}

template <typename... Args>
parser<Args...>::Iterator parser<Args...>::end()
{
    return Iterator(*this, true);
}

template <typename... Args>
void parser<Args...>::skip_lines()
{
    for (size_t i = 0; i < _skipped_lines; ++i)
    {
        std::string skippedLine;
        std::getline(_file, skippedLine);
    }
}
