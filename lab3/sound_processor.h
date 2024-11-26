#pragma once

#include <vector>
#include <string>
#include <memory>
#include "WAVfile.h"
#include "converters.h"

class SoundProcessor {
public:
    SoundProcessor(WAVFile& input_file, WAVFile& output_file, std::vector<std::unique_ptr<Converter>> converters);

    void process();

private:
    WAVFile& input_file_;
    WAVFile& output_file_;
    std::vector<std::unique_ptr<Converter>> converters_;
};
