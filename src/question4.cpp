#include "question.h"

#include <iostream>
#include <filesystem>

#include "FileData.h"
#include "Solution.h"

void question::question4(Config const& config)
{
    std::cout << " ------------ Générateurs aléatoires de solutions ------------ " << std::endl << std::endl;

    FileData fd;
    fd.load_from_path(std::filesystem::path(config.in_file));
    std::cout << config.in_file << " " << fd.get_bin_size() << " " << fd.get_item_num() << std::endl;

        std::cout << "générateur simple" << std::endl;
        
        Solution sol = fd.solve_simple();
        if(config.verbose)
        {
            std::cout << sol << std::endl;
        }
        else
        {
            std::cout << "nombre de bin : " << sol.get_nb_bin() << std::endl;
        }

        std::cout << "générateur first fit random" << std::endl;

        sol = fd.first_fit_random();
        if(config.verbose)
        {
            std::cout << sol << std::endl;
        }
        else
        {
            std::cout << "nombre de bin : " << sol.get_nb_bin() << std::endl;
        }
}