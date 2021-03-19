#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>

#include "FileData.h"

struct
{
    std::string in_file = "resources/binpack1d_00.txt";
} config;

int main(int argc, char *args[]) 
{
    std::ifstream input(config.in_file);

    FileData fd;
    fd.load_from_stream(input);

    input.close();

    std::cout << config.in_file << " " << fd.get_bin_size() << " " << fd.get_item_num() << std::endl;
    std::cout << "nombre bin min : " << fd.min_number_of_bin() << std::endl;

    Solution sol = fd.first_fit_decreasing();

    std::cout << sol;
    
    return 0;
}