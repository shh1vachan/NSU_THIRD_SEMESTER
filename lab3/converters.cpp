#include "converters.h"
#include "WAVfile.h"
#include <algorithm> 


MuteConverter::MuteConverter(int start, int end) : start_time_(start), end_time_(end) {}


void MuteConverter::apply(WAVFile &input, WAVFile &output, const std::vector<int> &params)
{
    int start_sample = start_time_ * input.get_sample_rate();
    int end_sample = end_time_ * input.get_sample_rate();

    const std::vector<int16_t>& input_samples = input.get_samples();
    std::vector<int16_t> output_samples(input_samples);

    for (int i = start_sample; i < end_sample && i < input_samples.size(); ++i)
        output_samples[i] = 0;

    output.set_samples(output_samples);
}


MixConverter::MixConverter(const WAVFile& additionalInput, int insert_position)
    : additionalInput_(additionalInput), insert_position_(insert_position) {}

void MixConverter::apply(WAVFile &input, WAVFile &output, const std::vector<int> &params) 
{
    const std::vector<int16_t>& input_samples = input.get_samples();
    const std::vector<int16_t>& additional_samples = additionalInput_.get_samples();

    std::vector<int16_t> output_samples(input_samples);

    int insert_sample = insert_position_ * input.get_sample_rate();
    for (size_t i = 0; i < additional_samples.size(); ++i) 
    {
        int target_sample_index = insert_sample + i;
        if (target_sample_index < output_samples.size()) 
            output_samples[target_sample_index] = std::clamp<int>(output_samples[target_sample_index] + additional_samples[i], -32768, 32767);
    }
    output.set_samples(output_samples);
}



CompressConverter::CompressConverter(int max_amplitude)
    : max_amplitude_(max_amplitude) {}


void CompressConverter::apply(WAVFile &input, WAVFile &output, const std::vector<int> &params)
{
    const std::vector<int16_t>& input_samples = input.get_samples();
    std::vector<int16_t> output_samples(input_samples);

    for (size_t i = 0; i < output_samples.size(); ++i)
        output_samples[i] = std::clamp(output_samples[i], static_cast<int16_t>(-max_amplitude_), static_cast<int16_t>(max_amplitude_));

    output.set_samples(output_samples);
}
