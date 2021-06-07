#include "question.h"

#include <iostream>
#include <filesystem>
#include <chrono>

#include "FileData.h"
#include "Solution.h"

void question::question3(Config const& config)
{
    std::cout << " ------------ Résolution linéaire ------------ " << std::endl << std::endl;

    FileData fd;
    fd.load_from_path(std::filesystem::path(config.in_file));
    std::cout << config.in_file << " " << fd.get_bin_size() << " " << fd.get_item_num() << std::endl;

    // résolution avec le solveur linéaire

    std::chrono::time_point start = std::chrono::high_resolution_clock::now();

    Solution sol = fd.solve_linear_problem();

    std::chrono::time_point stop = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    if(config.verbose)
    {
        std::cout << sol << std::endl;
    }
    else
    {
        std::cout << "nombre de bin : " << sol.get_nb_bin() << std::endl;
    }

    std::cout << "durée : " << duration.count() << " microsecondes" << std::endl;
}
