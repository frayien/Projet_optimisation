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

    // résolution avec first fit decreasing

    std::cout << " ------------ First Fit Decreasing ------------ " << std::endl << std::endl;

    Solution sol = fd.first_fit_decreasing();

    std::cout << sol << std::endl;

    std::wcout << L" ------------ Résolution linéaire ------------ " << std::endl << std::endl;

    sol = fd.solve_linear_problem();

    std::cout << sol << std::endl;
    
    return 0;
}