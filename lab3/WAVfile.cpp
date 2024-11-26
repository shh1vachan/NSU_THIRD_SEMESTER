#include "WAVfile.h"
#include "exceptions.h"
#include <string.h>


void WAVFile::set_filepath(const std::string& filepath) {filepath_ = filepath;}


const std::string& WAVFile::get_filepath() const {return filepath_;}


WAVFile::WAVFile(const std::string& filepath) : filepath_(filepath), sample_rate_(44100), bit_depth_(16), num_channels_(1), valid_format_(false) {}


WAVFile::WAVFile(const std::string& filepath, const std::vector<int16_t>& samples, int sample_rate)
    : filepath_(filepath), samples_(samples), sample_rate_(sample_rate), bit_depth_(16), num_channels_(1), valid_format_(true) {
    data_chunk_size_ = samples.size() * sizeof(int16_t);
}


bool WAVFile::read() 
{
    std::ifstream file(filepath_, std::ios::binary);
    if (!file.is_open())
        throw FileOpenException(filepath_);

    if (!read_header(file)) 
        throw InvalidFormatException();

    if (data_chunk_size_ == 0) 
        throw MissingDataException();

    samples_.resize(data_chunk_size_ / sizeof(int16_t));
    file.read(reinterpret_cast<char*>(samples_.data()), data_chunk_size_);

    if (file.gcount() != data_chunk_size_) 
        throw FileReadException(filepath_);

    return true;
}


bool WAVFile::read_header(std::ifstream& file) 
{
    char chunk_id[4];
    file.read(chunk_id, 4);
    if (std::string(chunk_id, 4) != "RIFF") 
        throw InvalidFormatException();

    file.seekg(4, std::ios::cur);

    char format[4];
    file.read(format, 4);
    if (std::string(format, 4) != "WAVE") 
        throw InvalidFormatException();

    while (file.read(chunk_id, 4)) 
    {
        uint32_t chunk_size;
        file.read(reinterpret_cast<char*>(&chunk_size), sizeof(chunk_size));

        if (std::string(chunk_id, 4) == "fmt ") 
        {
            uint16_t audio_format, num_channels, bits_per_sample;
            uint32_t sample_rate;

            file.read(reinterpret_cast<char*>(&audio_format), sizeof(audio_format));
            file.read(reinterpret_cast<char*>(&num_channels), sizeof(num_channels));
            file.read(reinterpret_cast<char*>(&sample_rate), sizeof(sample_rate));

            file.ignore(6); //skip byte rate and block allign

            file.read(reinterpret_cast<char*>(&bits_per_sample), sizeof(bits_per_sample));

            if (audio_format != 1 || num_channels != 1 || sample_rate != 44100 || bits_per_sample != 16) 
                throw InvalidFormatException();

        } 
        else if (std::string(chunk_id, 4) == "data") 
        {
            data_chunk_size_ = chunk_size;
            return true;
        } 
        else 
            file.seekg(chunk_size, std::ios::cur);
    }
    throw MissingDataException();
}


bool WAVFile::write() 
{
    std::ofstream file(filepath_, std::ios::binary);
    if (!file) 
        throw FileWriteException(filepath_);

    if (samples_.empty()) 
        throw MissingSamplesException();

    if (!write_header(file)) 
        throw FileWriteException(filepath_);

    file.write(reinterpret_cast<const char*>(samples_.data()), samples_.size() * sizeof(int16_t));

    if (!file) 
        throw FileWriteException(filepath_);

    return true;
}


bool WAVFile::write_header(std::ofstream& file) 
{
    uint32_t file_size = 4 + 8 + 16 + 8 + samples_.size() * sizeof(int16_t);
    uint32_t fmt_chunk_size = 16;
    uint32_t data_size = samples_.size() * sizeof(int16_t);

    file.write("RIFF", 4);
    file.write(reinterpret_cast<char*>(&file_size), 4);
    file.write("WAVE", 4);

    file.write("fmt ", 4);
    file.write(reinterpret_cast<char*>(&fmt_chunk_size), 4);
    uint16_t audio_format = 1;
    file.write(reinterpret_cast<char*>(&audio_format), 2);
    file.write(reinterpret_cast<char*>(&num_channels_), 2);
    file.write(reinterpret_cast<char*>(&sample_rate_), 4);
    uint32_t byte_rate = sample_rate_ * num_channels_ * bit_depth_ / 8;
    file.write(reinterpret_cast<char*>(&byte_rate), 4);
    uint16_t block_align = num_channels_ * bit_depth_ / 8;
    file.write(reinterpret_cast<char*>(&block_align), 2);
    file.write(reinterpret_cast<char*>(&bit_depth_), 2);

    file.write("data", 4);
    file.write(reinterpret_cast<char*>(&data_size), 4);

    return true;
}


const std::vector<int16_t>& WAVFile::get_samples() const 
{
    return samples_;
}


void WAVFile::set_samples(const std::vector<int16_t>& samples) 
{
    samples_ = samples;
}


int WAVFile::get_sample_rate() const 
{
    return sample_rate_;
}
