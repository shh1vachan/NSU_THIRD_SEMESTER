#pragma once

#include <stdexcept>
#include <string>

// Base exception for the sound processor
class SoundProcessorException : public std::runtime_error {
public:
    explicit SoundProcessorException(const std::string& message)
        : std::runtime_error(message) {}
};

// Exception for file-related errors
class FileException : public SoundProcessorException {
public:
    explicit FileException(const std::string& message)
        : SoundProcessorException("File error: " + message) {}
};

class FileOpenException : public FileException {
public:
    explicit FileOpenException(const std::string& filename)
        : FileException("Failed to open file: " + filename) {}
};

class FileReadException : public FileException {
public:
    explicit FileReadException(const std::string& filename)
        : FileException("Failed to read file: " + filename) {}
};

class FileWriteException : public FileException {
public:
    explicit FileWriteException(const std::string& filename)
        : FileException("Failed to write file: " + filename) {}
};

// Exception for invalid file format
class InvalidFormatException : public FileException {
public:
    explicit InvalidFormatException()
        : FileException("Invalid or unsupported WAV file format.") {}
};

// Exception for missing or invalid config
class ConfigException : public SoundProcessorException {
public:
    explicit ConfigException(const std::string& message)
        : SoundProcessorException("Config error: " + message) {}
};

class InvalidConfigLineException : public ConfigException {
public:
    explicit InvalidConfigLineException(int line_number, const std::string& details)
        : ConfigException("Error in config file at line " + std::to_string(line_number) + ": " + details) {}
};

class InvalidCommandException : public ConfigException {
public:
    explicit InvalidCommandException(const std::string& command)
        : ConfigException("Unknown command: " + command) {}
};

// Exception for WAV file processing
class WAVProcessingException : public SoundProcessorException {
public:
    explicit WAVProcessingException(const std::string& message)
        : SoundProcessorException("WAV processing error: " + message) {}
};

class MissingDataException : public WAVProcessingException {
public:
    explicit MissingDataException()
        : WAVProcessingException("No data available for processing.") {}
};

class MissingSamplesException : public WAVProcessingException {
public:
    explicit MissingSamplesException()
        : WAVProcessingException("WAV file has no samples to process.") {}
};
