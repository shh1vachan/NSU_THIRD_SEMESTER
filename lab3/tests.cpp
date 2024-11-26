#include <gtest/gtest.h>
#include "WAVfile.h"
#include "sound_processor.h"
#include "parser.h"
#include "exceptions.h"

// Tests for WAVFile class
TEST(WAVFileTests, ReadValidFileFunkorama) 
{
    WAVFile wav("funkorama.wav");
    ASSERT_NO_THROW(wav.read());
    EXPECT_GT(wav.get_samples().size(), 0);
}

TEST(WAVFileTests, WriteValidFile) 
{
    WAVFile wav("funkorama.wav");
    ASSERT_NO_THROW(wav.read());
    WAVFile output("output.wav", wav.get_samples(), wav.get_sample_rate());
    EXPECT_NO_THROW(output.write());
    EXPECT_GT(output.get_samples().size(), 0);
}

TEST(WAVFileTests, AccessSampleRateFunkorama) 
{
    WAVFile wav("funkorama.wav");
    ASSERT_NO_THROW(wav.read());
    EXPECT_EQ(wav.get_sample_rate(), 44100);
}

// Tests for SoundProcessor class
TEST(SoundProcessorTests, ProcessNoConverters) 
{
    WAVFile input("funkorama.wav");
    WAVFile output("output.wav", {}, input.get_sample_rate());
    ASSERT_NO_THROW(input.read());
    
    std::vector<std::unique_ptr<Converter>> converters;
    SoundProcessor processor(input, output, std::move(converters));
    
    EXPECT_NO_THROW(processor.process());
    EXPECT_EQ(input.get_samples(), output.get_samples());
}

TEST(SoundProcessorTests, ProcessMuteConverter) 
{
    WAVFile input("funkorama.wav");
    WAVFile output("output_mute.wav", {}, input.get_sample_rate());
    ASSERT_NO_THROW(input.read());
    
    std::vector<std::unique_ptr<Converter>> converters;
    converters.emplace_back(std::make_unique<MuteConverter>(2, 4));
    
    SoundProcessor processor(input, output, std::move(converters));
    EXPECT_NO_THROW(processor.process());
    EXPECT_EQ(output.get_sample_rate(), 44100);
}

TEST(SoundProcessorTests, ProcessMixConverter) 
{
    WAVFile input("funkorama.wav");
    WAVFile additional("Ouch-5.wav");
    WAVFile output("output_mix.wav", {}, input.get_sample_rate());
    
    ASSERT_NO_THROW(input.read());
    ASSERT_NO_THROW(additional.read());
    
    std::vector<std::unique_ptr<Converter>> converters;
    converters.emplace_back(std::make_unique<MixConverter>(additional, 5));
    
    SoundProcessor processor(input, output, std::move(converters));
    EXPECT_NO_THROW(processor.process());
    EXPECT_EQ(output.get_sample_rate(), 44100);
}

// Tests for ConfigParser class
TEST(ConfigParserTests, ParseValidConfig) 
{
    std::vector<WAVFile> additional_files = 
    {
        WAVFile("Ouch-5.wav"),
        WAVFile("Ouch-5.wav")
    };

    ASSERT_NO_THROW(additional_files[0].read());
    ASSERT_NO_THROW(additional_files[1].read());

    ConfigParser parser("config.txt", additional_files);

    EXPECT_NO_THROW
    ({
        auto converters = parser.parse_config();
        EXPECT_EQ(converters.size(), 4); // mute, mix, compress
    });
}

TEST(ConfigParserTests, ParseInvalidConfig) 
{
    std::vector<WAVFile> additional_files = 
    {
        WAVFile("district_four.wav")
    };

    ASSERT_NO_THROW(additional_files[0].read());
    
    ConfigParser parser("invalid_config.txt", additional_files);
    EXPECT_THROW(parser.parse_config(), InvalidConfigLineException);
}

// Tests for CompressConverter
TEST(SoundProcessorTests, ProcessCompressConverter) 
{
    WAVFile input("funkorama.wav");
    WAVFile output("output_compress.wav", {}, input.get_sample_rate());
    ASSERT_NO_THROW(input.read());
    
    std::vector<std::unique_ptr<Converter>> converters;
    converters.emplace_back(std::make_unique<CompressConverter>(5000));
    
    SoundProcessor processor(input, output, std::move(converters));
    EXPECT_NO_THROW(processor.process());
    EXPECT_EQ(output.get_sample_rate(), 44100);
}
