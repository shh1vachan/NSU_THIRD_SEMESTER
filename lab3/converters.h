#pragma once

#include "WAVfile.h"
#include <vector>
#include <string>
#include <memory>

// Base interface for all converters
class Converter {
public:
    virtual ~Converter() = default;
    virtual void apply(WAVFile& input, WAVFile& output, const std::vector<int>& params) = 0;
};

// Mute interval converter
class MuteConverter : public Converter {
private:
    int start_time_;
    int end_time_;
public:
    MuteConverter(int start, int end);
    void apply(WAVFile& input, WAVFile& output, const std::vector<int>& params) override;
};

// Mixer converter
class MixConverter : public Converter {
private:
    WAVFile additionalInput_;
    int insert_position_;
public:
    MixConverter(const WAVFile& additionalInput, int insert_position);
    void apply(WAVFile& input, WAVFile& output, const std::vector<int>& params) override;
};

// Compression converter
class CompressConverter : public Converter {
private:
    int max_amplitude_;
public:
    CompressConverter(int max_amplitude);
    void apply(WAVFile& input, WAVFile& output, const std::vector<int>& params) override;
};

// Factory for creating converters
class ConverterFactory 
{
public:
    static std::unique_ptr<Converter> createConverter
    (
        const std::string& type, 
        const std::vector<int>& params, 
        const WAVFile* additionalInput = nullptr
    )
     
    {
        if (type == "mute") 
            return std::make_unique<MuteConverter>(params[0], params[1]);
        
        else if (type == "mix") 
            return std::make_unique<MixConverter>(*additionalInput, params[0]);
        
        else if (type == "compress") 
            return std::make_unique<CompressConverter>(params[0]);
        
        throw std::invalid_argument("Unknown converter type: " + type);
    }
};
