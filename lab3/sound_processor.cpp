#include "sound_processor.h"
#include "exceptions.h"
#include <iostream>

SoundProcessor::SoundProcessor(WAVFile& input_file, WAVFile& output_file, std::vector<std::unique_ptr<Converter>> converters)
    : input_file_(input_file), output_file_(output_file), converters_(std::move(converters)) {}

void SoundProcessor::process() 
{
    WAVFile temp_input = input_file_;
    WAVFile temp_output = temp_input;

    if (converters_.empty()) 
    {
        std::cout << "No converters to apply. Copying input to output..." << std::endl;
        output_file_.set_samples(temp_input.get_samples());
        return;
    }

    for (size_t i = 0; i < converters_.size(); ++i) 
    {
        try 
        {
            converters_[i]->apply(temp_input, temp_output, {});
            temp_input = temp_output;
        } 
        catch (const std::exception& e) 
        {
            throw WAVProcessingException("Error applying converter at position " + std::to_string(i + 1) + ": " + e.what());
        }
    }

    if (temp_output.get_samples().empty()) 
    {
        throw MissingDataException();
    }

    output_file_.set_samples(temp_output.get_samples());
    output_file_.set_filepath(output_file_.get_filepath());

    if (!output_file_.write()) 
    {
        throw FileWriteException(output_file_.get_filepath());
    }

    std::cout << "Processing completed successfully. File saved to: " << output_file_.get_filepath() << std::endl;
}
