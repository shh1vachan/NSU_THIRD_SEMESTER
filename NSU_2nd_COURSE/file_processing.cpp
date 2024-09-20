#include "file_processing.h"
#include <fstream>
#include <sstream>
#include <cctype> 
#include <algorithm> 

file_processing::file_processing(const std::string& filename) : filename_(filename) {}

void file_processing::extract_from_txt() 
{
    std::ifstream file(filename_);
    if (!file.is_open()) 
    {
        std::cerr << "cannot open TXT file: " << filename_ << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);
        std::string word;

        //split the string by delimeter chars
        while (iss >> word) 
        {
            std::string clean_word;
            for (char c : word) 
            {
                if (std::isalnum(c)) 
                {
                    clean_word += c;
                } 
                else if (!clean_word.empty()) 
                {
                    words_map[clean_word]++;
                    words_counter++;
                    clean_word.clear(); 
                }
            if (!clean_word.empty()) {
                words_map[clean_word]++;
                words_counter++;
            }
        }
    }
    file.close();

    //convert data into a vector and calculate frequency as a percentage
    for (const auto& pair : words_map) 
    {
        float frequency_percent = (static_cast<float>(pair.second) / words_counter) * 100;
        data_.emplace_back(pair.first, pair.second, frequency_percent);
    }

    //sort data in descending order of frequencies as a percentage 
    std::sort(data_.begin(), data_.end(), [](const auto& a, const auto& b) 
    {
        return std::get<2>(a) > std::get<2>(b); // Сортируем по процентной частоте
    });
}

const std::vector<std::tuple<std::string, int, float>>& file_processing::get_data() const {
    return data_;
}
