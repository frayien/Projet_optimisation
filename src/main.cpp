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

    FileData fd = FileData::from_stream(input);

    input.close();

    std::cout << config.in_file << " " << fd.bin_size << " " << fd.item_num << std::endl;
    std::cout << "min nb bin " << fd.min_nb_bin() << std::endl;
    
    return 0;
}