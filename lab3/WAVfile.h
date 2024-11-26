#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>

class WAVFile
{
public:
    WAVFile(const std::string& filepath);
    WAVFile(const std::string& filepath, const std::vector<int16_t>& samples, int sample_rate = 44100);

    bool read();
    bool write();

    // Accessor methods
    const std::vector<int16_t>& get_samples() const;
    void set_samples(const std::vector<int16_t>& samples);
    int get_sample_rate() const;

    // WAV parameters methods
    int get_bit_depth() const { return bit_depth_; }
    int get_num_channels() const { return num_channels_; }
    int get_data_chunk_size() const { return data_chunk_size_; }

    // Filepath accessors
    void set_filepath(const std::string& filepath);
    const std::string& get_filepath() const;

private:
    // WAV file parameters
    std::string filepath_;
    std::vector<int16_t> samples_;
    int sample_rate_; 
    int bit_depth_;
    int num_channels_;
    int data_chunk_size_;
    bool valid_format_;

    // Helper methods for headers
    bool read_header(std::ifstream& file);
    bool write_header(std::ofstream& file);
};
