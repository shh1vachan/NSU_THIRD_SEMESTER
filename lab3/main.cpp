#include <iostream>
#include <vector>
#include <string>
#include "WAVfile.h"
#include "sound_processor.h"
#include "parser.h"
#include "exceptions.h"


int main(int argc, char* argv[]) 
{
    try 
    {
        if (argc < 2) 
        {
            ConfigParser::print_help();
            return 1;
        }

        std::string flag = argv[1];

        if (flag == "-h") 
        {
            ConfigParser::print_help();
            return 0;
        }

        if (flag != "-c") 
        {
            std::cerr << "Invalid flag: " << flag << std::endl;
            ConfigParser::print_help();
            return 1;
        }

        if (argc < 5) 
        {
            std::cerr << "Insufficient arguments for -c flag." << std::endl;
            ConfigParser::print_help();
            return 1;
        }

        std::string config_path = argv[2];
        std::string output_path = argv[3];
        std::string input_path = argv[4];

        std::vector<std::string> additional_file_paths;
        for (int i = 5; i < argc; ++i) 
            additional_file_paths.push_back(argv[i]);

        std::cout << "Reading primary input WAV file: " << input_path << std::endl;
        WAVFile input_file(input_path);
        if (!input_file.read()) 
            throw FileReadException(input_path);

        std::vector<WAVFile> additional_files;
        for (const auto& path : additional_file_paths) 
        {
            std::cout << "Reading additional WAV file: " << path << std::endl;
            WAVFile additional_file(path);

            if (!additional_file.read()) 
                throw FileReadException(path);
            
            additional_files.push_back(std::move(additional_file));
        }

        WAVFile output_file(output_path, {}, input_file.get_sample_rate());

        ConfigParser parser(config_path, additional_files);
        auto converters = parser.parse_config();

        SoundProcessor processor(input_file, output_file, std::move(converters));
        processor.process();

        std::cout << "Sound processing completed successfully!" << std::endl;
    } 
    
    catch (const FileOpenException& e) 
    {
        std::cerr << "File open error: " << e.what() << std::endl;
        return 1;
    } 
    catch (const FileReadException& e) 
    {
        std::cerr << "File read error: " << e.what() << std::endl;
        return 1;
    } 
    catch (const FileWriteException& e) 
    {
        std::cerr << "File write error: " << e.what() << std::endl;
        return 1;
    } 
    catch (const InvalidFormatException& e) 
    {
        std::cerr << "Invalid format error: " << e.what() << std::endl;
        return 1;
    } 
    catch (const ConfigException& e) 
    {
        std::cerr << "Configuration error: " << e.what() << std::endl;
        return 1;
    } 
    catch (const WAVProcessingException& e) 
    {
        std::cerr << "Processing error: " << e.what() << std::endl;
        return 1;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
