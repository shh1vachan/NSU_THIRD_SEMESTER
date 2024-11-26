#include "exceptions.h"

SoundProcessorException::SoundProcessorException(const std::string& message)
    : std::runtime_error(message) {}


FileException::FileException(const std::string& message)
    : SoundProcessorException("File error: " + message) {}


FileOpenException::FileOpenException(const std::string& filename)
    : FileException("Failed to open file: " + filename) {}


FileReadException::FileReadException(const std::string& filename)
    : FileException("Failed to read file: " + filename) {}


FileWriteException::FileWriteException(const std::string& filename)
    : FileException("Failed to write file: " + filename) {}


InvalidFormatException::InvalidFormatException()
    : FileException("Invalid or unsupported WAV file format.") {}


ConfigException::ConfigException(const std::string& message)
    : SoundProcessorException("Config error: " + message) {}


InvalidConfigLineException::InvalidConfigLineException(int line_number, const std::string& details)
    : ConfigException("Error in config file at line " + std::to_string(line_number) + ": " + details) {}


InvalidCommandException::InvalidCommandException(const std::string& command)
    : ConfigException("Unknown command: " + command) {}


WAVProcessingException::WAVProcessingException(const std::string& message)
    : SoundProcessorException("WAV processing error: " + message) {}


MissingDataException::MissingDataException()
    : WAVProcessingException("No data available for processing.") {}


MissingSamplesException::MissingSamplesException()
    : WAVProcessingException("WAV file has no samples to process.") {}
