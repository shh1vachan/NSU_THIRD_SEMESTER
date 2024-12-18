#pragma once

#include <iostream>
#include <string>
#include <exception>

class ErrorHandler : public std::exception
{
protected:
    std::string _text;

public:
    ErrorHandler(std::string text) : _text{ std::move(text) } {}

    const char *what() const noexcept override
    {
        return _text.c_str();
    }
};

class NotEnoughData final : public ErrorHandler
{
    size_t _line_num;

public:
    NotEnoughData(size_t line_num)
        : _line_num{ line_num }, ErrorHandler{ "Not enough data!" } {}

    size_t line_number() const { return _line_num; }
};

class FailedToReadData final : public ErrorHandler
{
    size_t _line_num;
    size_t _col_num;

public:
    FailedToReadData(size_t line_num, size_t col_num)
        : _line_num{ line_num }, _col_num{ col_num },
          ErrorHandler{ "Failed to read data!" } {}

    size_t line_number() const { return _line_num; }
    size_t col_number() const { return _col_num; }
};

class FileOpenError final : public ErrorHandler
{
public:
    FileOpenError() : ErrorHandler{ "The file could not be opened." } {}
};
