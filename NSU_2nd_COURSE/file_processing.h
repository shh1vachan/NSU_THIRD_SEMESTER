#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <map>
#include <algorithm>


class file_processing{
private:
    std::string filename_;
    std::vector<std::tuple<std::string, int, float>> data_;
    std::map<std::string, int> words_map;
    int words_counter = 0;

public:    
    
    file_processing(const std::string& filename);

    void extract_from_txt();

    const std::vector<std::tuple<std::string, int, float>>& get_data() const;
};