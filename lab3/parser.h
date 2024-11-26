#pragma once

#include <vector>
#include <string>
#include <memory>
#include "converters.h"
#include "WAVfile.h"

class ConfigParser {
public:
    ConfigParser(const std::string& config_path, const std::vector<WAVFile>& additional_files);

    std::vector<std::unique_ptr<Converter>> parse_config();

    static void print_help();

private:
    std::string config_path_;
    const std::vector<WAVFile>& additional_files_;  // Константная ссылка
};
